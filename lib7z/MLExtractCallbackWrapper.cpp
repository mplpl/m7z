// MLExtractCallbackWrapper.h

#include "StdAfx.h"
#include "MLExtractCallback.h"
#include "MLExtractCallbackWrapper.h"
#include <iostream>

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
    //TODO: convert time
    return cb->AskOverwrite(existName, 0, y, newName, 0, b, answer);
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

