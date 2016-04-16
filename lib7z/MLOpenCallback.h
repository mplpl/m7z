// OpenCallbackConsole.h

#ifndef __MLOPENCALLBACK_H
#define __MLOPENCALLBACK_H

#include "7zip/UI/Common/ArchiveOpenCallback.h"

class MLOpenCallback: public IOpenCallbackUI
{
public:
  INTERFACE_IOpenCallbackUI(;)

  bool PasswordIsDefined;
  bool PasswordWasAsked;
  UString Password;
  MLOpenCallback(): PasswordIsDefined(false), PasswordWasAsked(false) {}
};

#endif
