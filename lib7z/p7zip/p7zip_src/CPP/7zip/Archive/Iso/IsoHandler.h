// IsoHandler.h

#ifndef __ISO_HANDLER_H
#define __ISO_HANDLER_H

#include "../../../Common/MyCom.h"

#include "../IArchive.h"

#include <iconv.h>

#include "IsoIn.h"
#include "IsoItem.h"

namespace NArchive {
namespace NIso {

class CHandler:
  public IInArchive,
  public IInArchiveGetStream,
  public ISetProperties,
  public CMyUnknownImp
{
  CMyComPtr<IInStream> _stream;
  CInArchive _archive;
  iconv_t _convBaseToUtf8 = (iconv_t)-1;
public:
  MY_QUERYINTERFACE_BEGIN2(IInArchive)
  MY_QUERYINTERFACE_ENTRY(IInArchiveGetStream)
  MY_QUERYINTERFACE_ENTRY(ISetProperties)
  MY_QUERYINTERFACE_END
  MY_ADDREF_RELEASE
    
  INTERFACE_IInArchive(;)
  STDMETHOD(GetStream)(UInt32 index, ISequentialInStream **stream);
    
  ~CHandler()
  {
      if (_convBaseToUtf8 != (iconv_t)-1)
          iconv_close(_convBaseToUtf8);
  }
  STDMETHOD(SetProperties)(const wchar_t * const *names, const PROPVARIANT *values, UInt32 numProps);
};

}}

#endif
