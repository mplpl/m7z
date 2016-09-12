// OpenCallbackConsole.cpp

#include "StdAfx.h"

#include "MLListCallbackWrapper.h"

using namespace lib7z;

HRESULT MLListCallbackWrapper::Open_CryptoGetTextPassword(BSTR *password)
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


// ***************************************** IGNORED CALLBACKS *****************************************

HRESULT MLListCallbackWrapper::Open_CheckBreak() { return S_OK; }
HRESULT MLListCallbackWrapper::Open_SetTotal(const UInt64 *x, const UInt64 *y) { return S_OK; }
HRESULT MLListCallbackWrapper::Open_SetCompleted(const UInt64 *, const UInt64 *) { return S_OK; }
HRESULT MLListCallbackWrapper::Open_Finished() { return S_OK; }