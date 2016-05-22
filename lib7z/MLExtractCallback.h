// MLExtractCallback.h

#ifndef __MLEXTRACTCALLBACK_H
#define __MLEXTRACTCALLBACK_H

namespace AskOverwrite
{
    enum Answer
    {
        kYes,
        kYesToAll,
        kNo,
        kNoToAll,
        kAutoRename,
        kCancel
    };
}


class MLExtractCallback
{
public:
  virtual int SetTotal(unsigned long long size) = 0;
  virtual int DecompressingItem(const wchar_t *name, bool isFolder, int askExtractMode, const unsigned long long *position) = 0;
  virtual int SetCompleted(const unsigned long long *completeValue) = 0;
  virtual int SetOperationResult(int operationResult, int kind) = 0;
  virtual int FinishArchive() = 0;

  virtual int AskOverwrite(
      const wchar_t *existName, const time_t *existTime, const unsigned long long *existSize,
      const wchar_t *newName, const time_t *newTime, const unsigned long long *newSize,
      int *answer) = 0;

  virtual const wchar_t *GetPassword() = 0;

  virtual int ThereAreNoFiles() = 0;
  virtual int MessageError(const wchar_t *message) = 0;
};

#endif
