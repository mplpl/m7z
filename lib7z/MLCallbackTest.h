// MLCallbackTest.h

#ifndef __ML_CALLBACK_TEST_H
#define __ML_CALLBACK_TEST_H

#include "MLUpdateCallback.h"
#include "MLExtractCallback.h"
#include <string>

class MLCallbackTest: public MLUpdateCallback, public MLExtractCallback
{

public:

  int SetTotal(unsigned long long size);
  int CompressingItem(const wchar_t *name, bool isAnti);
  int DecompressingItem(const wchar_t *name, bool isFolder, int askExtractMode, const unsigned long long *position);
  int SetCompleted(const unsigned long long *completeValue);
  int SetOperationResult(int operationResult, int kind);
  int FinishArchive();

  int AskOverwrite(
      const wchar_t *existName, const time_t *existTime, const unsigned long long *existSize,
      const wchar_t *newName, const time_t *newTime, const unsigned long long *newSize,
      int *answer);

  const wchar_t *GetPassword();

  int CanNotFindError(const wchar_t *name, int systemError);
  int OpenFileError(const wchar_t *name, int systemError);

  int MessageError(const wchar_t *message);
  int ThereAreNoFiles();
  
  void setPassword(std::wstring newpass);

private:
  std::wstring pass;
};

#endif
