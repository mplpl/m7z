// UpdateCallbackWrapper.h

#ifndef __MLUPDATE_CALLBACK_WRAPPER_H
#define __MLUPDATE_CALLBACK_WRAPPER_H

#include "7zip/UI/Common/Update.h"
#include "MLUpdateCallback.h"

class MLUpdateCallbackWrapper: public IUpdateCallbackUI2
{

public:
  bool PasswordIsDefined;
  UString Password;
  bool AskPassword;
  MLUpdateCallback *cb;

  MLUpdateCallbackWrapper(MLUpdateCallback &cb):
      cb(&cb),
      PasswordIsDefined(false),
      AskPassword(false)
      {}

  ~MLUpdateCallbackWrapper() { Finilize(); }

  INTERFACE_IUpdateCallbackUI2(;)
};

#endif
