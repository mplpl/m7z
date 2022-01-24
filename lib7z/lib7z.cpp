// Main.cpp
// Copyright @ 2016-2022 MPL. All rights reserved.

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
#include <sstream>

#include <iconv.h>

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
   
LIB7ZRC MLListArchive(std::wstring archiveNameW, std::vector<DirectoryItem> &retValue, MLListCallback &callback, std::wstring encoding)
{    
    UString archiveName = archiveNameW.c_str();
    NWindows::NFile::NFind::CFileInfo fi;
    if (!fi.Find(archiveName) || fi.IsDir())
    {
      return Lib7zReturnCode::CANTFINDARCHIVE;
    }
    
    HRESULT result;
    
    CCodecs codecs;
    if (codecs.Load() != S_OK)
    {
        return Lib7zReturnCode::CANTLOADCODECS;
    }
    
    CIntVector formatIndices;
    CObjectVector<COpenType> types;
    MLListCallbackWrapper openCallback(callback);
    COpenOptions openOptions;
    CObjectVector<CProperty> props;
    if (!encoding.empty())
    {
        CProperty prop ={L"cps", encoding.c_str()};
        props.Add(prop);
    }
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
        char modtime[32];
        if ((propTime.filetime.dwLowDateTime == 0) && (propTime.filetime.dwHighDateTime == 0)) {
            modtime[0] = 0;
        }
        else
        {
            FILETIME localFileTime;
            FileTimeToLocalFileTime(&propTime.filetime, &localFileTime);
            if (!ConvertFileTimeToString(localFileTime, modtime, true, true)) modtime[0] = 0;
            //format is: YYYY-MM-DD hh:mm:ss
        }
        
        RINOK(arc.Archive->GetProperty(i, kpidCTime, &propTime));
        char createtime[32];
        if ((propTime.filetime.dwLowDateTime == 0) && (propTime.filetime.dwHighDateTime == 0)) {
            createtime[0] = 0;
        }
        else
        {
            FILETIME localFileTime;
            FileTimeToLocalFileTime(&propTime.filetime, &localFileTime);
            if (!ConvertFileTimeToString(localFileTime, createtime, true, true)) createtime[0] = 0;
            //format is: YYYY-MM-DD hh:mm:ss
        }
        
        RINOK(arc.Archive->GetProperty(i, kpidATime, &propTime));
        char accesstime[32];
        if ((propTime.filetime.dwLowDateTime == 0) && (propTime.filetime.dwHighDateTime == 0)) {
            accesstime[0] = 0;
        }
        else
        {
            FILETIME localFileTime;
            FileTimeToLocalFileTime(&propTime.filetime, &localFileTime);
            if (!ConvertFileTimeToString(localFileTime, accesstime, true, true)) accesstime[0] = 0;
            //format is: YYYY-MM-DD hh:mm:ss
        }
        
        NWindows::NCOM::CPropVariant propEncrypted;
        RINOK(arc.Archive->GetProperty(i, kpidEncrypted, &propEncrypted));
        bool encrypted = false;
        if (propEncrypted.boolVal) {
            encrypted = true;
        }
        
        NWindows::NCOM::CPropVariant propMethod;
        RINOK(arc.Archive->GetProperty(i, kpidMethod, &propMethod));
        UString method;
        if (propMethod.vt == VT_BSTR) {
            method = (UString)propMethod.bstrVal;
        }
        
        DirectoryItem di(std::wstring(sPath), size, sizePacked,
                         std::string(createtime),
                         std::string(modtime),
                         std::string(accesstime),
                         std::string(s),
                         encrypted,
                         std::wstring(method));
        retValue.push_back(di);
    }
    
    return 0;
}

bool DoesNameMatchWildcards(UString name,
                            std::vector<std::wstring> &exclusionWildcards,
                            std::vector<UString> &removedPathes) {
    
    for (std::vector<std::wstring>::iterator it = exclusionWildcards.begin();
         it < exclusionWildcards.end(); it++)
    {
        UString nameOnly = ExtractFileNameFromPath(name);
        bool match = DoesWildcardMatchName(it->c_str(), nameOnly);
        if (match) {
            removedPathes.push_back(name);
            return true;
        }
    }
    
    // now check if the item is not inside of already removed folder
    for (std::vector<UString>::iterator it = removedPathes.begin();
         it < removedPathes.end(); it++)
    {
        bool found = name.IsPrefixedBy((*it + L"/"));
        if (found) return true;
    }
    return false;
}

LIB7ZRC MLExtractFromArchive(std::wstring archiveNameW, std::wstring outDirW,
                             std::vector<std::wstring> files,
                             MLExtractCallback &callback, std::wstring workDir,
                             std::wstring encoding,
                             std::vector<std::wstring> exclusionWildcards)
{
    UString archiveName = archiveNameW.c_str();
    UString outDir = outDirW.c_str();
    
    NWindows::NFile::NFind::CFileInfo fi;
    if (!fi.Find(archiveName) || fi.IsDir())
    {
        return Lib7zReturnCode::CANTFINDARCHIVE;
    }
    
    HRESULT result;
    
    CCodecs codecs;
    if (codecs.Load() != S_OK)
    {
        return Lib7zReturnCode::CANTLOADCODECS;
    }
    
    CIntVector formatIndices;
    CObjectVector<COpenType> types;
    CObjectVector<CProperty> props;
    if (!encoding.empty())
    {
        CProperty prop ={L"cps", encoding.c_str()};
        props.Add(prop);
    }
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
    
    std::vector<UString> removedPathes;
    for (UInt32 i = 0; i < numItems; i++)
    {
        UString filePath;
        RINOK(arc.GetItemPath(i, filePath));
        
        if (DoesNameMatchWildcards(filePath, exclusionWildcards, removedPathes)) continue;
        
        if (files.size())
        {
            // firstly simply check if given items in on the list (files)
            if (std::find(files.begin(), files.end(), std::wstring(filePath)) == files.end()) {
                // not found
                
                // now check if it is substring (subdirectory) of files on the list (using files2)
                bool found = false;
                for (std::vector<std::wstring>::iterator it = files2.begin(); it < files2.end(); it++)
                {
                    auto res = std::mismatch(it->begin(), it->end(), std::wstring(filePath).begin());
                    if (res.first == it->end())
                    {
                        found = true;
                        break;
                    }
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
        return Lib7zReturnCode::CANTCREATEOUTDIR;
    }
    
    MLExtractCallbackWrapper *ecs = new MLExtractCallbackWrapper(callback);
    // hold ComPtr reference for autorelease
    CMyComPtr<IFolderArchiveExtractCallback> ecb = ecs;
    
    CArchiveExtractCallback *extractCallbackSpec = new CArchiveExtractCallback();
    // hold ComPtr reference for autorelease - without this bz2 extract crashes (EXC_BAD_ACCESS)
    CMyComPtr<IProgress> ec = (IArchiveExtractCallback*)extractCallbackSpec;
    
    // removePathParts allows cutting out base path of extracted path
    // for instance if I'm unpacking file x/y/z.txt, I only want z.txt in dest directory
    // and not full x/y/z.txt - removePathParts defines what is x/y
    // as there is only one parameters, all the items to be extrcted need to be in
    // the same base direcory in the archive and I'm only getting it from the first
    // item to unpack
    // if unpacking all, removePathParts should be left empty
    UStringVector removePathParts;
    if (files.size())
    {
        SplitPathToParts(files.begin()->c_str(), removePathParts);
        if (!removePathParts.IsEmpty())
        {
            removePathParts.DeleteBack();
        }
    }
    CExtractNtOptions ntOptions;
    extractCallbackSpec->Init(ntOptions,
                              NULL,
                              &arc,
                              ecs,
                              false, false,
                              outDir,
                              removePathParts, false,
                              0);
    
    // pathMode allows to strip pathes and only extract files to destination directory
    // unfortunatelly when extracting a dir it takes items out of this dir so that
    // they will be located directly at destination - therefore I'm not using it here
    extractCallbackSpec->InitForMulti(false, NExtract::NPathMode::EEnum::kFullPaths, NExtract::NOverwriteMode::EEnum::kAsk);
    
    try {
        result = archive->Extract(&realIndices.Front(), realIndices.Size(), false, extractCallbackSpec);
    
        if (result == S_OK)
        {
            extractCallbackSpec->SetDirsTimes();
        }
    } catch (...) {
        result = Lib7zReturnCode::FAIL; // turns into M7Z_RC_FAIL
    }
    
    callback.FinishArchive();
    
    return result;
}


LIB7ZRC MLGenericCommand(std::wstring command, std::wstring archiveNameW,
                         std::vector<std::wstring> filesW, MLUpdateCallback &cb,
                         bool encryptHeader, int compressionLevel, std::wstring workDir,
                         std::wstring encoding,
                         bool storeCreatedTime, bool moveToArchive,
                         std::vector<std::wstring> exclusionWildcards)
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
        return Lib7zReturnCode::CANTLOADCODECS;
    }
    
    UStringVector commandStrings;
    commandStrings.Add(command.c_str());
    std::wstringstream wstr;
    wstr << L"-mx" << compressionLevel;
    commandStrings.Add(wstr.str().c_str());
    if (encryptHeader)
    {
        commandStrings.Add(L"-mhe");
    }
    commandStrings.Add(archiveName);
    for (std::vector<UString>::iterator it = files.begin(); it < files.end(); it++)
    {
        commandStrings.Add(*it);
    }
    if (!encoding.empty())
    {
        std::wstringstream wstrMcps;
        wstrMcps << L"-mcps=" << encoding.c_str();
        commandStrings.Add(wstrMcps.str().c_str());
    }
    if (storeCreatedTime) {
        commandStrings.Add(L"-mtc");
    }
    if (moveToArchive) {
        commandStrings.Add(L"-sdel");
    }
    
    for (std::vector<std::wstring>::iterator it = exclusionWildcards.begin();
         it < exclusionWildcards.end(); it++)
    {
        std::wstringstream filter;
        filter << L"-xr!" << (*it);
        commandStrings.Add(filter.str().c_str());
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
        return Lib7zReturnCode::UOINITFAILED;
    }
    
    uo.WorkingDir = workDir.c_str();
    CUpdateErrorInfo errorInfo;
    
    try {
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
        
    } catch (...) {
        return Lib7zReturnCode::FAIL; // turns into M7Z_RC_FAIL
    }
}



LIB7ZRC MLAddToArchive(std::wstring archiveNameW, std::vector<std::wstring> filesW, MLUpdateCallback &cb,
                       bool encryptHeader, int compressionLevel, std::wstring workDir,
                       std::wstring encoding, bool storeCreatedTime, bool moveToArchive,
                       std::vector<std::wstring> exclusionWildcards)
{
    return MLGenericCommand(L"a", archiveNameW, filesW, cb,
                            encryptHeader, compressionLevel,
                            workDir, encoding,
                            storeCreatedTime, moveToArchive,
                            exclusionWildcards);
}


LIB7ZRC MLDeleteFromArchive(std::wstring archiveNameW, std::vector<std::wstring> filesW, MLUpdateCallback &cb,
                        std::wstring workDir, std::wstring encoding)
{
    std::vector<std::wstring> exclusionWildcards;
    return MLGenericCommand(L"d", archiveNameW, filesW, cb, false,
                            9, workDir, encoding, false, false,
                            exclusionWildcards);
}

LIB7ZRC MLRenameItemsInArchive(std::wstring archiveNameW, std::vector<std::wstring> fromToList,
                              MLUpdateCallback &cb, std::wstring workDir, std::wstring encoding)
{
    std::vector<std::wstring> exclusionWildcards;
    return MLGenericCommand(L"rn", archiveNameW, fromToList, cb, false,
                            9, workDir, encoding, false, false,
                            exclusionWildcards);
}

int do_one(unsigned int count, const char * const *names, void *arg) {
    std::vector<std::wstring> *arr = (std::vector<std::wstring> *)arg;
    std::wstringstream s;
    s << iconv_canonicalize(names[0]);
    arr->push_back(s.str());
    return 0;
}

const std::vector<std::wstring> MLSupportedEncodings() {
    std::vector<std::wstring> ret;
    iconvlist(&do_one, &ret);
    return ret;
}

}
