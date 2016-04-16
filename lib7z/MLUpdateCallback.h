// UpdateCallback.h

#ifndef __MLUPDATE_CALLBACK_H
#define __MLUPDATE_CALLBACK_H

#include <cwchar>

class MLUpdateCallback
{

public:

  virtual int SetTotal(unsigned long long size) = 0;
  virtual int CompressingItem(const wchar_t *name, bool isAnti) = 0;
  virtual int SetCompleted(const unsigned long long *completeValue) = 0;
  virtual int SetOperationResult(int x) = 0;
  virtual int FinishArchive() = 0;

  virtual const wchar_t *GetPassword() = 0;

  virtual int CanNotFindError(const wchar_t *name, int systemError) = 0;
  virtual int OpenFileError(const wchar_t *name, int systemError) = 0;
};

#endif
