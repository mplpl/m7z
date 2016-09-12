// Main.cpp

#include "StdAfx.h"

#if defined( _7ZIP_LARGE_PAGES)
#include "../../../../C/Alloc.h"
#endif

#include "Common/MyInitGuid.h"
#include "Windows/ErrorMsg.h"
#include "Windows/PropVariantConv.h"
#include "7zip/UI/Common/ArchiveCommandLine.h"

#include "lib7z.h"
#include "MLListCallbackWrapper.h"
#include "MLExtractCallbackWrapper.h"
#include "MLUpdateCallbackWrapper.h"

#include <vector>
#include <string>


namespace lib7z
{


static void GetAttribString(DWORD wa, bool isDir, char *s)
{
    if (isDir) wa |= FILE_ATTRIBUTE_DIRECTORY;
    s[0] = ((wa & FILE_ATTRIBUTE_DIRECTORY) != 0) ? 'D': '.';
    s[1] = ((wa & FILE_ATTRIBUTE_READONLY)  != 0) ? 'R': '.';
    s[2] = ((wa & FILE_ATTRIBUTE_HIDDEN)    != 0) ? 'H': '.';
    s[3] = ((wa & FILE_ATTRIBUTE_SYSTEM)    != 0) ? 'S': '.';
    s[4] = ((wa & FILE_ATTRIBUTE_ARCHIVE)   != 0) ? 'A': '.';
    s[5] = 0;
}

std::wstring GetErrorMessage(HRESULT result)
{
    switch(result)
    {
        case OperationReturnCode::OK:                  return L"OK";
        case OperationReturnCode::ABORT:               return L"Aborted";
        case OperationReturnCode::FERROR:              return L"Can not open file as archive";
        case OperationReturnCode::OUTOFMEMORY:         return L"Can't allocate required memory";
        case OperationReturnCode::CANTLOADCODECS:      return L"Can't load codecs";
        case OperationReturnCode::UOINITFAILED:        return L"Can't initialize operation";
        case OperationReturnCode::CANTCREATEOUTDIR:    return L"Can't create output dir";
        case OperationReturnCode::CANTFINDARCHIVE:     return L"Can't find archive";
        default:
            return std::wstring(NWindows::NError::MyFormatMessage(result));
    }
}

std::wstring GetExtractOperationErrorMessage(int result)
{
    switch (result) {
        case ExtractOperationResult::kOK:                   return L"OK";
        case ExtractOperationResult::kUnsupportedMethod:    return L"Unsuported compression method";
        case ExtractOperationResult::kDataError:            return L"Data error";
        case ExtractOperationResult::kCRCError:             return L"CRC error";
        case 1000 + ExtractOperationResult::kDataError:     return L"Data error in encrypted file. Wrong password?";
        case 1000 + ExtractOperationResult::kCRCError:      return L"CRC error in encrypted file. Wrong password?";
        case ExtractOperationResult::kUnavailable:          return L"Archive is not available";
        default:                                            return L"Unknown error";
    }
}

ORC MLListArchive(std::wstring archiveNameW, std::vector<DirectoryItem> &retValue, MLListCallback &callback)
{
    UString archiveName = archiveNameW.c_str();
    NWindows::NFile::NFind::CFileInfo fi;
    if (!fi.Find(archiveName) || fi.IsDir())
    {
      return OperationReturnCode::CANTFINDARCHIVE;
    }
    
    HRESULT result;
    
    CCodecs codecs;
    if (codecs.Load() != S_OK)
    {
        return OperationReturnCode::CANTLOADCODECS;
    }
    
    CIntVector formatIndices;
    CObjectVector<COpenType> types;
    MLListCallbackWrapper openCallback(callback);
    COpenOptions openOptions;
    CObjectVector<CProperty> props;

    openOptions.codecs = &codecs;
    openOptions.filePath = archiveName;
    openOptions.excludedFormats = &formatIndices;
    openOptions.types = &types;
    openOptions.props = &props;

    CArchiveLink archiveLink;
    RINOK(archiveLink.Open2(openOptions, &openCallback));
    
    const CArc &arc = archiveLink.Arcs.Back();
    IInArchive *archive = arc.Archive;
    UInt32 numItems;
    RINOK(archive->GetNumberOfItems(&numItems));
    for (UInt32 i = 0; i < numItems; i++)
    {
        UString sPath;
        RINOK(arc.GetItemPath(i, sPath));
        
        bool isFolder = false;
        NWindows::NCOM::CPropVariant propIsFolder;
        RINOK(arc.Archive->GetProperty(i, kpidIsDir, &propIsFolder));
        if (propIsFolder.vt == VT_BOOL)
        {
            isFolder = VARIANT_BOOLToBool(propIsFolder.boolVal);
        }
        
        NWindows::NCOM::CPropVariant propSize;
        RINOK(arc.Archive->GetProperty(i, kpidSize, &propSize));
        UInt64 size = -1;
        if (propSize.vt != VT_EMPTY) ConvertPropVariantToUInt64(propSize, size);
        
        NWindows::NCOM::CPropVariant propSizePacked;
        RINOK(arc.Archive->GetProperty(i, kpidPackSize, &propSizePacked));
        UInt64 sizePacked = -1;
        if (propSizePacked.vt != VT_EMPTY) ConvertPropVariantToUInt64(propSizePacked, sizePacked);
        
        UInt32 iAttrs;
        NWindows::NCOM::CPropVariant propAttrs;
        RINOK(arc.Archive->GetProperty(i, kpidAttrib, &propAttrs));
        iAttrs = (propAttrs.vt == VT_EMPTY ? 0 : propAttrs.ulVal);
        char s[8];
        GetAttribString(iAttrs, isFolder, s);
        
        NWindows::NCOM::CPropVariant propTime;
        RINOK(arc.Archive->GetProperty(i, kpidMTime, &propTime));
        char ftime[32];
        if ((propTime.filetime.dwLowDateTime == 0) && (propTime.filetime.dwHighDateTime == 0)) {
            ftime[0] = 0;
        }
        else
        {
            FILETIME localFileTime;
            FileTimeToLocalFileTime(&propTime.filetime, &localFileTime);
            if (!ConvertFileTimeToString(localFileTime, ftime, true, true)) ftime[0] = 0;
            //format is: YYYY-MM-DD hh:mm:ss
        }
        
        DirectoryItem di(std::wstring(sPath), size, sizePacked, std::string(ftime), std::string(s));
        retValue.push_back(di);
    }
    
    return 0;
}


ORC MLExtractFromArchive(std::wstring archiveNameW, std::wstring outDirW, std::vector<std::wstring> files,
                        MLExtractCallback &callback, std::wstring workDir)
{
    UString archiveName = archiveNameW.c_str();
    UString outDir = outDirW.c_str();
    
    NWindows::NFile::NFind::CFileInfo fi;
    if (!fi.Find(archiveName) || fi.IsDir())
    {
        return OperationReturnCode::CANTFINDARCHIVE;
    }
    
    HRESULT result;
    
    CCodecs codecs;
    if (codecs.Load() != S_OK)
    {
        return OperationReturnCode::CANTLOADCODECS;
    }
    
    CIntVector formatIndices;
    CObjectVector<COpenType> types;
    CObjectVector<CProperty> props;
    MLListCallbackWrapper openCallback(callback);
    COpenOptions openOptions;
    openOptions.codecs = &codecs;
    openOptions.filePath = archiveName;
    openOptions.types = &types;
    openOptions.excludedFormats = &formatIndices;
    openOptions.props = &props;

    CArchiveLink archiveLink;
    RINOK(archiveLink.Open2(openOptions, &openCallback));
    const CArc &arc = archiveLink.Arcs.Back();
    IInArchive *archive = arc.Archive;
    
    CRecordVector<UInt32> realIndices;
    UInt32 numItems;
    RINOK(archive->GetNumberOfItems(&numItems));
    
    std::vector<std::wstring> files2;
    for (std::vector<std::wstring>::iterator it = files.begin(); it < files.end(); it++)
    {
        files2.push_back(*it + L"/");
    }
    
    for (UInt32 i = 0; i < numItems; i++)
    {
        UString filePath;
        RINOK(arc.GetItemPath(i, filePath));
        if (files.size())
        {
            // firstly simply check if given items in on the list (files)
            if (std::find(files.begin(), files.end(), std::wstring(filePath)) == files.end()) {
                // not found
                
                // now check if it is substring (subdirectory) of files on the list (using files2)
                bool found = false;
                for (std::vector<std::wstring>::iterator it = files.begin(); it < files.end(); it++)
                {
                    auto res = std::mismatch(it->begin(), it->end(), std::wstring(filePath).begin());
                    if (res.first == it->end())
                        found = true;
                        break;
                }
                if (!found) continue;
            }
        }
        realIndices.Add(i);
    }
    //if (realIndices.Size() == 0)
    //{
        //return ecs->ThereAreNoFiles();
    //}
    
    if (!outDir.IsEmpty() && !NWindows::NFile::NDir::CreateComplexDir(outDir))
    {
        return OperationReturnCode::CANTCREATEOUTDIR;
    }
    
    UStringVector removePathParts;
    MLExtractCallbackWrapper *ecs = new MLExtractCallbackWrapper(callback);
    CArchiveExtractCallback *extractCallbackSpec = new CArchiveExtractCallback();
    CExtractNtOptions ntOptions;
    extractCallbackSpec->Init(ntOptions,
                              NULL,
                              &arc,
                              ecs,
                              false, false,
                              outDir,
                              removePathParts, false,
                              0);
    result = archive->Extract(&realIndices.Front(), realIndices.Size(), false, extractCallbackSpec);
    callback.FinishArchive();
    
    return result;
}


ORC MLGenericCommand(std::wstring command, std::wstring archiveNameW, std::vector<std::wstring> filesW, MLUpdateCallback &cb,
                     bool encryptHeader, int compressionLevel, std::wstring workDir)
{
    UString archiveName = archiveNameW.c_str();
    std::vector<UString> files;
    for (std::vector<std::wstring>::iterator it = filesW.begin(); it < filesW.end(); it++)
    {
        files.push_back(UString(it->c_str()));
    }

    CCodecs codecs;
    if (codecs.Load() != S_OK)
    {
        return OperationReturnCode::CANTLOADCODECS;
    }
    
    UStringVector commandStrings;
    commandStrings.Add(command.c_str());
    commandStrings.Add((L"-mx" + std::to_wstring(compressionLevel)).c_str());
    if (encryptHeader)
    {
        commandStrings.Add(L"-mhe");
    }
    commandStrings.Add(archiveName);
    for (std::vector<UString>::iterator it = files.begin(); it < files.end(); it++)
    {
        commandStrings.Add(*it);
    }
    CArcCmdLineOptions options;
    CArcCmdLineParser parser;
    parser.Parse1(commandStrings, options);
    parser.Parse2(options);
    
    CObjectVector<COpenType> formatIndices;
    CUpdateOptions &uo = options.UpdateOptions;
    MLListCallbackWrapper openCallback(cb);
    MLUpdateCallbackWrapper callback(cb) ;
    
    if (!uo.InitFormatIndex(&codecs, formatIndices, options.ArchiveName))
    {
        return OperationReturnCode::UOINITFAILED;
    }
    
    uo.WorkingDir = workDir.c_str();
    CUpdateErrorInfo errorInfo;
    
    HRESULT res = UpdateArchive(&codecs,
                                formatIndices,
                                options.ArchiveName,
                                options.Censor,
								uo,
                                errorInfo,
								&openCallback,
								&callback,
								true);
    
    return res;
}



ORC MLAddToArchive(std::wstring archiveNameW, std::vector<std::wstring> filesW, MLUpdateCallback &cb,
                      bool encryptHeader, int compressionLevel, std::wstring workDir)
{
    return MLGenericCommand(L"a", archiveNameW, filesW, cb, encryptHeader, compressionLevel, workDir);
}


ORC MLDeleteFromArchive(std::wstring archiveNameW, std::vector<std::wstring> filesW, MLUpdateCallback &cb,
                        std::wstring workDir)
{
    return MLGenericCommand(L"d", archiveNameW, filesW, cb, false, 9, workDir);
}
    
}
