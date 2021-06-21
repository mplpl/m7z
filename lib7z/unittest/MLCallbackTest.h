// MLCallbackTest.h
// Copyright @ 2016-2021 MPL. All rights reserved.

#include <iostream>

#ifndef __ML_CALLBACK_TEST_H
#define __ML_CALLBACK_TEST_H

#include "../MLUpdateCallback.h"
#include "../MLExtractCallback.h"
#include "../MLListCallback.h"
#include <string>


class MLCallbackTest: public lib7z::MLUpdateCallback, public lib7z::MLExtractCallback
{
    
public:

    int SetTotal(unsigned long long size);
    int CompressingItem(const wchar_t *name, bool isAnti, int mode);
    int DecompressingItem(const wchar_t *name, bool isFolder, int mode, const unsigned long long *position);
    int SetCompleted(const unsigned long long *completeValue);
    int SetOperationResult(int operationResult, int kind);
    int FinishArchive();
    
    int AskOverwrite(
                     const wchar_t *existName, const time_t *existTime, const unsigned long long *existSize,
                     const wchar_t *newName, const time_t *newTime, const unsigned long long *newSize,
                     int *answer);
    
    const std::wstring GetPassword();
    
    int OpenFileError(const wchar_t *name, int systemError);
    
    int MessageError(const wchar_t *message);
    int ThereAreNoFiles();
    
    void setPassword(std::wstring newpass);
    
    int askOverwriteCounter;
private:
    std::wstring pass;
};

#endif
