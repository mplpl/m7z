// OpenCallbackConsole.cpp

#include "StdAfx.h"

#include "MLOpenCallback.h"

//#include <iostream>
//using namespace std;

HRESULT MLOpenCallback::Open_CheckBreak()
{
  return S_OK;
}

HRESULT MLOpenCallback::Open_SetTotal(const UInt64 *x, const UInt64 *y)
{
  //wcout << "Open_SetTotal " << *x << " " << *y << endl;
  return Open_CheckBreak();
}

HRESULT MLOpenCallback::Open_SetCompleted(const UInt64 *, const UInt64 *)
{
  //wcout << ".";
  //wcout.flush();
  return Open_CheckBreak();
}

HRESULT MLOpenCallback::Open_CryptoGetTextPassword(BSTR *password)
{
  PasswordWasAsked = true;
  RINOK(Open_CheckBreak());
  if (!PasswordIsDefined)
  {
    Password = L"";  //TODO
    PasswordIsDefined = true;
  }
  return StringToBstr(Password, password);
}

HRESULT MLOpenCallback::Open_GetPasswordIfAny(UString &password)
{
  if (PasswordIsDefined)
    password = Password;
  return S_OK;
}

bool MLOpenCallback::Open_WasPasswordAsked()
{
  return PasswordWasAsked;
}

void MLOpenCallback::Open_ClearPasswordWasAskedFlag()
{
  PasswordWasAsked = false;
}
