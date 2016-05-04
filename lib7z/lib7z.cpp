// Main.cpp

#include "StdAfx.h"

#if defined( _7ZIP_LARGE_PAGES)
#include "../../../../C/Alloc.h"
#endif

#include "Common/MyInitGuid.h"
#include "7zip/UI/Console/UpdateCallbackConsole.h"
#include "7zip/UI/Console/ExtractCallbackConsole.h"
#include "Windows/Error.h"
#include "Windows/PropVariant.h"
#include "Windows/PropVariantConversions.h"
#include "Windows/FileDir.h"
#include "7zip/UI/Common/ArchiveCommandLine.h"

#include "lib7z.h"
#include "MLOpenCallback.h"
#include "MLExtractCallbackWrapper.h"
#include "MLUpdateCallbackWrapper.h"

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace NWindows;
using namespace NFile;

static const char kEmptyAttribChar = '.';
static void GetAttribString(DWORD wa, bool isDir, char *s)
{
  s[0] = ((wa & FILE_ATTRIBUTE_DIRECTORY) != 0 || isDir) ? 'D' : kEmptyAttribChar;
  s[1] = ((wa & FILE_ATTRIBUTE_READONLY) != 0) ? 'R': kEmptyAttribChar;
  s[2] = ((wa & FILE_ATTRIBUTE_HIDDEN) != 0) ? 'H': kEmptyAttribChar;
  s[3] = ((wa & FILE_ATTRIBUTE_SYSTEM) != 0) ? 'S': kEmptyAttribChar;
  s[4] = ((wa & FILE_ATTRIBUTE_ARCHIVE) != 0) ? 'A': kEmptyAttribChar;
  s[5] = '\0';
}

static BOOL IsFileTimeZero(CONST FILETIME *lpFileTime)
{
  return (lpFileTime->dwLowDateTime == 0) && (lpFileTime->dwHighDateTime == 0);
}

#define X_CANTLOADCODECS ((HRESULT)300001)
#define X_UOINITFAILED ((HRESULT)300002)
#define X_CANTCREATEOUTDIR ((HRESULT)300003)

const wchar_t *GetErrorMessage(HRESULT result)
{
	if (result == E_ABORT)
  {
    return L"Aborted";
	}
  else if (result == S_FALSE)
	{
		return L"Can not open file as archive";
	}
	else if (result == E_OUTOFMEMORY)
  {
		return L"Can't allocate required memory";
  }
  else if (result == X_CANTLOADCODECS)
  {
		return L"Can't load codecs";
  }
  else if (result == X_UOINITFAILED)
  {
		return L"Can't initialize operation";
  }
  else if (result == X_CANTCREATEOUTDIR)
  {
		return L"Can't create output dir";
  }
  else
		return NError::MyFormatMessage(result);

  return L"Unknown";
}

HRESULT MLListArchive(std::wstring archiveNameW, std::vector<DirectoryItem> &retValue)
{
  UString archiveName = archiveNameW.c_str();
  NFile::NFind::CFileInfoW fi;
  if (!fi.Find(archiveName) || fi.IsDir())
  {
    return S_FALSE;
  }

	HRESULT result;

	CCodecs *codecs = new CCodecs;
	CMyComPtr<IUnknown> compressCodecsInfo = codecs;
	result = codecs->Load();
	if (result != S_OK)
	{
	  return X_CANTLOADCODECS;
	}

	CIntVector formatIndices;

  MLOpenCallback openCallback;

  CArchiveLink archiveLink;
  result = archiveLink.Open2(codecs, formatIndices, false, NULL, archiveName, &openCallback);
  if (result != S_OK) return result;

  const CArc &arc = archiveLink.Arcs.Back();
  IInArchive *archive = arc.Archive;
  UInt32 numItems;
  RINOK(archive->GetNumberOfItems(&numItems));
  for (UInt32 i = 0; i < numItems; i++)
  {
    UString sPath;
    HRESULT res = arc.GetItemPath(i, sPath);
    RINOK(res);

    bool isFolder;
    RINOK(IsArchiveItemFolder(archive, i, isFolder));


    NCOM::CPropVariant propSize;
    RINOK(arc.Archive->GetProperty(i, kpidSize, &propSize));
    int size = -1;
    if (propSize.vt != VT_EMPTY) size = ConvertPropVariantToUInt64(propSize);

    NCOM::CPropVariant propSizePacked;
    RINOK(arc.Archive->GetProperty(i, kpidPackSize, &propSizePacked));
    int sizePacked = -1;
    if (propSizePacked.vt != VT_EMPTY) sizePacked = ConvertPropVariantToUInt64(propSizePacked);

    UInt32 iAttrs;
    NCOM::CPropVariant propAttrs;
    RINOK(arc.Archive->GetProperty(i, kpidAttrib, &propAttrs));
    iAttrs = (propAttrs.vt == VT_EMPTY ? 0 : propAttrs.ulVal);
    char s[8];
    GetAttribString(iAttrs, isFolder, s);

    NCOM::CPropVariant propTime;
    RINOK(arc.Archive->GetProperty(i, kpidMTime, &propTime));
    char ftime[32];
    if (IsFileTimeZero(&propTime.filetime))
  	  ftime[0] = 0;
    else
    {
        FILETIME localFileTime;
        if (!FileTimeToLocalFileTime(&propTime.filetime, &localFileTime))
          throw "FileTimeToLocalFileTime error";
        if (!ConvertFileTimeToString(localFileTime, ftime, true, true)) ftime[0] = 0;
    }

    DirectoryItem di(std::wstring(sPath), size, sizePacked, std::string(ftime), std::string(s));
    retValue.push_back(di);
  }

  return 0;
}


HRESULT MLDecompressArchive(std::wstring archiveNameW, std::wstring outDirW, std::vector<std::wstring> files, MLExtractCallback &callback)
{
  UString archiveName = archiveNameW.c_str();
  UString outDir = outDirW.c_str();

  NFile::NFind::CFileInfoW fi;
  if (!fi.Find(archiveName) || fi.IsDir())
  {
    //callback.OpenResult(archiveNameW.c_str(), S_FALSE);
    return S_FALSE;
  }

	CCodecs *codecs = new CCodecs;
	CMyComPtr<IUnknown> compressCodecsInfo = codecs;
	HRESULT result = codecs->Load();
	if (result != S_OK)
	{
	  return X_CANTLOADCODECS;
	}

	CIntVector formatIndices;

	MLOpenCallback openCallback;

	MLExtractCallbackWrapper *ecs = new MLExtractCallbackWrapper(callback);

	CArchiveLink archiveLink;
	result = archiveLink.Open2(codecs, formatIndices, false, NULL, archiveName, &openCallback);
	if (result != S_OK) return result;
	const CArc &arc = archiveLink.Arcs.Back();
	IInArchive *archive = arc.Archive;

	CRecordVector<UInt32> realIndices;
	UInt32 numItems;
	RINOK(archive->GetNumberOfItems(&numItems));

	for (UInt32 i = 0; i < numItems; i++)
	{
	  UString filePath;
	  RINOK(arc.GetItemPath(i, filePath));
	  //bool isFolder;
	  //RINOK(IsArchiveItemFolder(archive, i, isFolder));
	  //if (!wildcardCensor.CheckPath(filePath, !isFolder))
		//continue;
        if (files.size() &&
            std::find(files.begin(), files.end(), std::wstring(filePath)) == files.end())
        {
            continue;
        }
        realIndices.Add(i);
	}
	if (realIndices.Size() == 0)
	{
	  return ecs->ThereAreNoFiles();
	}

	if (!outDir.IsEmpty())
	    if (!NFile::NDirectory::CreateComplexDirectory(outDir))
	    {
	      //HRESULT res = ::GetLastError();
	      //if (res == S_OK)
	      //  res = E_FAIL;
	      //errorMessage = ((UString)L"Can not create output directory ") + outDir;
	      return X_CANTCREATEOUTDIR;
	    }

	UStringVector removePathParts;

	CArchiveExtractCallback *extractCallbackSpec = new CArchiveExtractCallback;
	//extractCallbackSpec->InitForMulti(false, NExtract::NPathMode::kFullPathnames, NExtract::NOverwriteMode::kWithoutPrompt);
	extractCallbackSpec->Init(
			      NULL,
			      &arc,
				    ecs,
			      false, false, false,
			      outDir,
			      removePathParts,
			      0);
	result = archive->Extract(&realIndices.Front(), realIndices.Size(), false, extractCallbackSpec);
	callback.FinishArchive();
  return result;
}


HRESULT MLCompressArchive(std::wstring archiveNameW, std::vector<std::wstring> filesW, MLUpdateCallback &cb)
{
  UString archiveName = archiveNameW.c_str();
  std::vector<UString> files;
  for (std::vector<std::wstring>::iterator it = filesW.begin(); it < filesW.end(); it++)
  {
    files.push_back(UString(it->c_str()));
  }

	HRESULT result;

	CCodecs *codecs = new CCodecs;
	CMyComPtr<IUnknown> compressCodecsInfo = codecs;
	result = codecs->Load();
	if (result != S_OK)
	{
		return X_CANTLOADCODECS;
	}

	CIntVector formatIndices;

	UStringVector commandStrings;
	commandStrings.Add(L"a");
	commandStrings.Add(archiveName);
  for (std::vector<UString>::iterator it = files.begin(); it < files.end(); it++)
  {
    commandStrings.Add(*it);
  }
	CArchiveCommandLineOptions options;
	CArchiveCommandLineParser parser;
	parser.Parse1(commandStrings, options);
	parser.Parse2(options);

	CUpdateOptions &uo = options.UpdateOptions;
	MLOpenCallback openCallback;
	MLUpdateCallbackWrapper callback = MLUpdateCallbackWrapper(cb) ;

	if (!uo.Init(codecs, formatIndices, options.ArchiveName))
	{
		return X_UOINITFAILED;
	}

	CUpdateErrorInfo errorInfo;

	HRESULT res = UpdateArchive(codecs,
			options.WildcardCensor, uo,
			errorInfo, &openCallback, &callback);

  //if (res)
  //{
  //    g_StdOut << "xxxxxx " << errorInfo.FileName << " " << errorInfo.FileName2 << " "<< errorInfo.Message << "\n";
  //}
  return res;
}
