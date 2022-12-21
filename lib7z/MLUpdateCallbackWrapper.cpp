// UpdateCallbackConsole.cpp
// Copyright @ 2016-2022 MPL. All rights reserved.

#include "StdAfx.h"
#include "MLUpdateCallbackWrapper.h"

#include "Windows/Synchronization.h"
static NWindows::NSynchronization::CCriticalSection g_CriticalSection;
#define MT_LOCK NWindows::NSynchronization::CCriticalSectionLock lock(g_CriticalSection);

using namespace lib7z;


HRESULT MLUpdateCallbackWrapper::FinishArchive(const CFinishArchiveStat &st)
{
    MT_LOCK
    return cb->FinishArchive();
}

HRESULT MLUpdateCallbackWrapper::SetTotal(UInt64 size)
{
    MT_LOCK
    return cb->SetTotal(size);
}

HRESULT MLUpdateCallbackWrapper::SetCompleted(const UInt64 *completeValue)
{
    // not using it any more, as it is hard to interpret for add/update operation
    //MT_LOCK
    //return cb->SetCompleted(completeValue);
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::SetRatioInfo(const UInt64 *inSize, const UInt64 * outSize) {
    // this callback is better than MLUpdateCallbackWrapper::SetCompleted to describe the current
    // operation progress as it shows exact amount of data read and compressed
    MT_LOCK
    return cb->SetCompleted(inSize);
}

HRESULT MLUpdateCallbackWrapper::GetStream(const wchar_t *name, bool isDir, bool isAnti, UInt32 mode)
{
    MT_LOCK
    return cb->CompressingItem(name, isAnti, mode);
}

HRESULT MLUpdateCallbackWrapper::OpenFileError(const FString &path, DWORD systemError)
{
    MT_LOCK
    return cb->OpenFileError(path, systemError) == 0 ? 1 : 0;
}

HRESULT MLUpdateCallbackWrapper::SetOperationResult(Int32 x)
{
    MT_LOCK
    return cb->SetOperationResult(x, 2);
}

HRESULT MLUpdateCallbackWrapper::CryptoGetTextPassword2(Int32 *passwordIsDefined, BSTR *password)
{
    MT_LOCK
    const std::wstring pass = cb->GetPassword();
    if (!pass.empty())
    {
        *passwordIsDefined = 1;
        return StringToBstr(pass.c_str(), password);
    }
    else
    {
        *passwordIsDefined = 0;
        return S_OK;
    }
}

HRESULT MLUpdateCallbackWrapper::CryptoGetTextPassword(BSTR *password)
{
    MT_LOCK
    const std::wstring pass = cb->GetPassword();
    return StringToBstr(pass.c_str(), password);
}




// ***************************************** IGNORED CALLBACKS *****************************************


HRESULT MLUpdateCallbackWrapper::OpenResult(const CCodecs *codecs, const CArchiveLink &arcLink, const wchar_t *name, HRESULT result) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::StartScanning() { return S_OK; }
HRESULT MLUpdateCallbackWrapper::ScanProgress(const CDirItemsStat &st, const FString &path, bool isDir) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::FinishScanning(const CDirItemsStat &st) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::StartArchive(const wchar_t *name, bool updating) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::CheckBreak() { return S_OK; }
HRESULT MLUpdateCallbackWrapper::SetNumItems(UInt64 numFiles) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::WriteSfx(const wchar_t *name, UInt64 size)  { return S_OK; }
HRESULT MLUpdateCallbackWrapper::ShowDeleteFile(const wchar_t *name, bool isDir) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::ReadingFileError(const FString &path, DWORD systemError) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::StartOpenArchive(const wchar_t *name) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::ReportExtractResult(Int32 opRes, Int32 isEncrypted, const wchar_t *name) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::ReportUpdateOpeartion(UInt32 op, const wchar_t *name, bool isDir) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::DeletingAfterArchiving(const FString &path, bool isDir) { return S_OK; }
HRESULT MLUpdateCallbackWrapper::FinishDeletingAfterArchiving() { return S_OK; }
HRESULT MLUpdateCallbackWrapper::ScanError(const FString &path, DWORD systemError) { return S_OK; }
