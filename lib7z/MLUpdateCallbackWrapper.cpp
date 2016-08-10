//UpdateCallbackConsole.cpp

#include "StdAfx.h"
#include "MLUpdateCallbackWrapper.h"

#ifndef _7ZIP_ST
#include "Windows/Synchronization.h"
static NWindows::NSynchronization::CCriticalSection g_CriticalSection;
#define MT_LOCK NWindows::NSynchronization::CCriticalSectionLock lock(g_CriticalSection);
#else
#define MT_LOCK
#endif

HRESULT MLUpdateCallbackWrapper::OpenResult(const wchar_t *name, HRESULT result)
{
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::StartScanning()
{
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::ScanProgress(UInt64  numFolders , UInt64 numFiles, const wchar_t *path)
{
    return CheckBreak();
}

HRESULT MLUpdateCallbackWrapper::CanNotFindError(const wchar_t *name, DWORD systemError)
{
    return cb->CanNotFindError(name, systemError);
}

HRESULT MLUpdateCallbackWrapper::FinishScanning()
{
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::StartArchive(const wchar_t *name, bool updating)
{
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::FinishArchive()
{
    return cb->FinishArchive();
}

HRESULT MLUpdateCallbackWrapper::CheckBreak()
{
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::Finilize()
{
    MT_LOCK
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::SetNumFiles(UInt64 numFiles)
{
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::SetTotal(UInt64 size)
{
    MT_LOCK
    cb->SetTotal(size);
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::SetCompleted(const UInt64 *completeValue)
{
    MT_LOCK
    return cb->SetCompleted(completeValue);
}

HRESULT MLUpdateCallbackWrapper::SetRatioInfo(const UInt64 *inSize, const UInt64 * outSize)
{
    return S_OK;
}

HRESULT MLUpdateCallbackWrapper::GetStream(const wchar_t *name, bool isAnti)
{
    MT_LOCK
    return cb->CompressingItem(name, isAnti);
}

HRESULT MLUpdateCallbackWrapper::OpenFileError(const wchar_t *name, DWORD systemError)
{
    MT_LOCK
    return cb->OpenFileError(name, systemError);
}

HRESULT MLUpdateCallbackWrapper::SetOperationResult(Int32 x)
{
    return cb->SetOperationResult(x, 2);
}

HRESULT MLUpdateCallbackWrapper::CryptoGetTextPassword2(Int32 *passwordIsDefined, BSTR *password)
{
    return CryptoGetTextPassword(password);
}

HRESULT MLUpdateCallbackWrapper::CryptoGetTextPassword(BSTR *password)
{
    const wchar_t *pass = cb->GetPassword();
    if (pass)
    {
        return StringToBstr(pass, password);
    }
    else
    {
        return S_OK;
    }
}
