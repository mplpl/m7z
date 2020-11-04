// OpenCallbackConsole.cpp
// Copyright @ 2016-2020 MPL. All rights reserved.

#include "StdAfx.h"

#include "MLListCallbackWrapper.h"

using namespace lib7z;

HRESULT MLListCallbackWrapper::Open_CryptoGetTextPassword(BSTR *password)
{
    const std::wstring pass = cb->GetPassword();
    return StringToBstr(pass.c_str(), password);
}


// ***************************************** IGNORED CALLBACKS *****************************************

HRESULT MLListCallbackWrapper::Open_CheckBreak() { return S_OK; }
HRESULT MLListCallbackWrapper::Open_SetTotal(const UInt64 *x, const UInt64 *y) { return S_OK; }
HRESULT MLListCallbackWrapper::Open_SetCompleted(const UInt64 *, const UInt64 *) { return S_OK; }
HRESULT MLListCallbackWrapper::Open_Finished() { return S_OK; }
