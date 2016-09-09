// OpenCallbackConsole.h

#ifndef __MLOPENCALLBACK_H
#define __MLOPENCALLBACK_H

#include "7zip/UI/Common/ArchiveOpenCallback.h"

namespace lib7z
{

class MLOpenCallback: public IOpenCallbackUI
{
public:
    INTERFACE_IOpenCallbackUI(;)
    
    bool PasswordIsDefined;
    UString Password;
    MLOpenCallback(): PasswordIsDefined(false) {}

    virtual ~MLOpenCallback() {}
};
    
}
#endif
