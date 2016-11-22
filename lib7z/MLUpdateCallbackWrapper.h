// UpdateCallbackWrapper.h
// Copyright @ 2016 MPL. All rights reserved.

#ifndef __MLUPDATE_CALLBACK_WRAPPER_H
#define __MLUPDATE_CALLBACK_WRAPPER_H

#include "7zip/UI/Common/Update.h"
#include "MLUpdateCallback.h"

namespace lib7z
{

class MLUpdateCallbackWrapper: public IUpdateCallbackUI2
{
    
public:
    
    MLUpdateCallbackWrapper(MLUpdateCallback &cb):
        cb(&cb) {}
    
    virtual ~MLUpdateCallbackWrapper() { }
    
    INTERFACE_IUpdateCallbackUI2(;)

private:
    MLUpdateCallback *cb;
    
};
    
}

#endif
