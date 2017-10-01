// MLExtractCallbackWrapper.h
// Copyright @ 2016 MPL. All rights reserved.

#include "StdAfx.h"
#include "MLExtractCallback.h"
#include "MLExtractCallbackWrapper.h"
#include <iostream>
#include <Windows/TimeUtils.h>

#include "Windows/Synchronization.h"
static NWindows::NSynchronization::CCriticalSection g_CriticalSection;
#define MT_LOCK NWindows::NSynchronization::CCriticalSectionLock lock(g_CriticalSection);

using namespace std;
using namespace lib7z;

STDMETHODIMP
MLExtractCallbackWrapper::SetTotal(UInt64 x)
{
    MT_LOCK
    return cb->SetTotal(x);
}

STDMETHODIMP
MLExtractCallbackWrapper::SetCompleted(const UInt64 *x)
{
    MT_LOCK
    return cb->SetCompleted(x);
}

STDMETHODIMP
MLExtractCallbackWrapper::AskOverwrite(
    const wchar_t *existName, const FILETIME *x, const UInt64 *y,
    const wchar_t *newName, const FILETIME *a, const UInt64 *b,
    Int32 *answer)
{
    MT_LOCK
    UInt32 time1;
    UInt32 time2;
    NWindows::NTime::FileTimeToUnixTime(*x, time1);
    NWindows::NTime::FileTimeToUnixTime(*a, time2);
    time_t utime1 = time1;
    time_t utime2 = time2;
    return cb->AskOverwrite(existName, &utime1, y, newName, &utime2, b, answer);
}

STDMETHODIMP
MLExtractCallbackWrapper::PrepareOperation(
    const wchar_t *name, Int32 isFolder, Int32 askExtractMode,
    const UInt64 *position)
{
    MT_LOCK
    return cb->DecompressingItem(name, isFolder, askExtractMode, position);
}

STDMETHODIMP
MLExtractCallbackWrapper::MessageError(const wchar_t *message)
{
    MT_LOCK
    return cb->MessageError(message);
}

STDMETHODIMP
MLExtractCallbackWrapper::SetOperationResult(Int32 operationResult, Int32 encrypted)
{
    MT_LOCK
    if (encrypted && (operationResult == NArchive::NExtract::NOperationResult::kCRCError ||
                      operationResult == NArchive::NExtract::NOperationResult::kDataError))
    {
        operationResult = operationResult + 100;
    }
    return cb->SetOperationResult(operationResult, 1);
}

STDMETHODIMP
MLExtractCallbackWrapper::CryptoGetTextPassword(BSTR *password)
{
    MT_LOCK
    const wchar_t *pass = cb->GetPassword();
    if (pass)
    {
        return StringToBstr(pass, password);
    }
    else
    {
        StringToBstr(L"", password);
        return S_OK;
    }
}

