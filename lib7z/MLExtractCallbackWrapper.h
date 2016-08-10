// MLExtractCallbackWrapper.h

#ifndef __MLEXTRACTCALLBACKWRAPPER_H
#define __MLEXTRACTCALLBACKWRAPPER_H

#include "MLExtractCallback.h"
#include "Common/MyString.h"
//#include "../../Common/FileStreams.h"
//#include "../../IPassword.h"
#include "7zip/Archive/IArchive.h"
#include "7zip/UI/Common/ArchiveExtractCallback.h"

//#include "MLExtractMode.h"
//#include "IFileExtractCallback.h"
//#include "OpenArchive.h"

class MLExtractCallbackWrapper:
public IExtractCallbackUI,
public ICryptoGetTextPassword,
public CMyUnknownImp
{
public:
    
    MLExtractCallbackWrapper(MLExtractCallback &cb): cb(&cb)
    {};
    
    MY_QUERYINTERFACE_BEGIN2(IFolderArchiveExtractCallback)
    MY_QUERYINTERFACE_ENTRY(ICryptoGetTextPassword)
    MY_QUERYINTERFACE_END
    MY_ADDREF_RELEASE
    
    STDMETHOD(SetTotal)(UInt64 total);
    STDMETHOD(SetCompleted)(const UInt64 *completeValue);
    
    // IFolderArchiveExtractCallback
    STDMETHOD(AskOverwrite)(
                            const wchar_t *existName, const FILETIME *existTime, const UInt64 *existSize,
                            const wchar_t *newName, const FILETIME *newTime, const UInt64 *newSize,
                            Int32 *answer);
    STDMETHOD (PrepareOperation)(const wchar_t *name, bool isFolder, Int32 askExtractMode, const UInt64 *position);
    
    STDMETHOD(MessageError)(const wchar_t *message);
    STDMETHOD(SetOperationResult)(Int32 operationResult, bool encrypted);
    
    HRESULT BeforeOpen(const wchar_t *name);
    HRESULT OpenResult(const wchar_t *name, HRESULT result, bool encrypted);
    HRESULT ThereAreNoFiles();
    HRESULT ExtractResult(HRESULT result);
    
    
    HRESULT SetPassword(const UString &password);
    STDMETHOD(CryptoGetTextPassword)(BSTR *password);
    
private:
    MLExtractCallback *cb;
};

#endif
