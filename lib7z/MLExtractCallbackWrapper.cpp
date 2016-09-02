// MLExtractCallbackWrapper.h

#include "StdAfx.h"
#include "MLExtractCallback.h"
#include "MLExtractCallbackWrapper.h"
#include <iostream>

using namespace std;

STDMETHODIMP MLExtractCallbackWrapper::SetTotal(UInt64 x)
{
    cb->SetTotal(x);
    return S_OK;
}

STDMETHODIMP MLExtractCallbackWrapper::SetCompleted(const UInt64 *x)
{
    return cb->SetCompleted(x);
}

STDMETHODIMP MLExtractCallbackWrapper::AskOverwrite(
                                                    const wchar_t *existName, const FILETIME *x, const UInt64 *y,
                                                    const wchar_t *newName, const FILETIME *a, const UInt64 *b,
                                                    Int32 *answer)
{
    return cb->AskOverwrite(existName, 0, y, newName, 0, b, answer);
}

STDMETHODIMP MLExtractCallbackWrapper::PrepareOperation(const wchar_t *name, bool  isFolder ,
                                                        Int32 askExtractMode, const UInt64 *position)
{
    return cb->DecompressingItem(name, isFolder, askExtractMode, position);
}

STDMETHODIMP MLExtractCallbackWrapper::MessageError(const wchar_t *message)
{
    return cb->MessageError(message);
}

STDMETHODIMP MLExtractCallbackWrapper::SetOperationResult(Int32 operationResult, bool encrypted)
{
    if (encrypted && (operationResult == NArchive::NExtract::NOperationResult::kCRCError ||
                      operationResult == NArchive::NExtract::NOperationResult::kDataError))
    {
        operationResult = operationResult + 1000;
    }
    return cb->SetOperationResult(operationResult, 1);
}

HRESULT MLExtractCallbackWrapper::SetPassword(const UString &password)
{
    return S_OK;
}

STDMETHODIMP MLExtractCallbackWrapper::CryptoGetTextPassword(BSTR *password)
{
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



HRESULT MLExtractCallbackWrapper::BeforeOpen(const wchar_t *name)
{
    return S_OK;
}

HRESULT MLExtractCallbackWrapper::OpenResult(const wchar_t *name, HRESULT result, bool encrypted)
{
    return S_OK;
}

HRESULT MLExtractCallbackWrapper::ThereAreNoFiles()
{
    return cb->ThereAreNoFiles();
}

HRESULT MLExtractCallbackWrapper::ExtractResult(HRESULT result)
{
    return result;
}
