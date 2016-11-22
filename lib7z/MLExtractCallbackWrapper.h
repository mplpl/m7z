// MLExtractCallbackWrapper.h
// Copyright @ 2016 MPL. All rights reserved.

#ifndef __MLEXTRACTCALLBACKWRAPPER_H
#define __MLEXTRACTCALLBACKWRAPPER_H

#include "MLExtractCallback.h"
#include "7zip/UI/Common/ArchiveExtractCallback.h"

namespace lib7z
{

class MLExtractCallbackWrapper:
    public IFolderArchiveExtractCallback,
    public ICryptoGetTextPassword,
    public CMyUnknownImp
{
public:
    
    MLExtractCallbackWrapper(MLExtractCallback &cb):
        cb(&cb) {};
    
    MY_QUERYINTERFACE_BEGIN2(IFolderArchiveExtractCallback)
    MY_QUERYINTERFACE_ENTRY(ICryptoGetTextPassword)
    MY_QUERYINTERFACE_END
    MY_ADDREF_RELEASE
    
    INTERFACE_IFolderArchiveExtractCallback(;);
    INTERFACE_IProgress(;)
    
    STDMETHOD(CryptoGetTextPassword)(BSTR *password);
    
private:
    MLExtractCallback *cb;
};
    
}

#endif
