// OpenCallbackConsole.cpp

#include "StdAfx.h"

#include "MLOpenCallback.h"

using namespace lib7z;

HRESULT MLOpenCallback::Open_CryptoGetTextPassword(BSTR *password)
{
    if (!PasswordIsDefined)
    {
        Password = L"";  //TODO
        PasswordIsDefined = true;
    }
    return StringToBstr(Password, password);
}


// ***************************************** IGNORED CALLBACKS *****************************************

HRESULT MLOpenCallback::Open_CheckBreak() { return S_OK; }
HRESULT MLOpenCallback::Open_SetTotal(const UInt64 *x, const UInt64 *y) { return S_OK; }
HRESULT MLOpenCallback::Open_SetCompleted(const UInt64 *, const UInt64 *) { return S_OK; }
HRESULT MLOpenCallback::Open_Finished() { return S_OK; }