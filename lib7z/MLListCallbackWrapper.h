// OpenCallbackConsole.h

#ifndef __MLOPENCALLBACK_H
#define __MLOPENCALLBACK_H

#include "MLListCallback.h"
#include "7zip/UI/Common/ArchiveOpenCallback.h"

namespace lib7z
{

class MLListCallbackWrapper: public IOpenCallbackUI
{
public:
    INTERFACE_IOpenCallbackUI(;)
    
    MLListCallbackWrapper(MLListCallback &cb): cb(&cb) {}
    virtual ~MLListCallbackWrapper() {}
    
private:
    MLListCallback *cb;
};
    
}
#endif
