// MLCallbackTest.cpp

#include "MLCallbackTest.h"
#include <iostream>
#include <fstream>
#include "../lib7z.h"

using namespace std;


std::wofstream devnull("/dev/null");
#define OUT devnull
#define S_OK 0
#define S_FALSE 1

int MLCallbackTest::SetTotal(unsigned long long size)
{
    OUT << "[SetTotal] " << size << endl;
    return S_OK;
}

int MLCallbackTest::CanNotFindError(const wchar_t *name, int systemError)
{
    OUT << "[CanNotFindError] Cannot find  " << name << " (error " << systemError << ")" << endl;
    return S_FALSE;
}

int MLCallbackTest::FinishArchive()
{
    OUT << "[FinishArchive] Done" << endl;
    return S_OK;
}

int MLCallbackTest::SetCompleted(const unsigned long long *completeValue)
{
    OUT << "[SetCompleted] " << *completeValue << endl;
    //wcout << ",";
    OUT.flush();
    return S_OK;
}

int MLCallbackTest::CompressingItem(const wchar_t *name, bool isAnti, int mode)
{
    if(isAnti)
    {
        OUT << "[CompressingItem] Anti item    " << name << endl;
    }
    else
    {
        OUT << "[CompressingItem] ";
        switch (mode)
        {
            case lib7z::CompressingItemMode::kAdd:          OUT << "Adding "; break;
            case lib7z::CompressingItemMode::kUpdate:       OUT << "Update "; break;
            case lib7z::CompressingItemMode::kAnalyze:      OUT << "Analyze "; break;
            case lib7z::CompressingItemMode::kReplicate:    OUT << "Replicate "; break;
            case lib7z::CompressingItemMode::kRepack:       OUT << "Repack "; break;
            case lib7z::CompressingItemMode::kSkip:         OUT << "Skip "; break;
            case lib7z::CompressingItemMode::kDelete:       OUT << "Delete "; break;
            case lib7z::CompressingItemMode::kHeader:       OUT << "Header "; break;
        }
        OUT << name << endl;
    }
    return S_OK;
}

int MLCallbackTest::OpenFileError(const wchar_t *name, int systemError)
{
    OUT << "[OpenFileError] " << name << " " << systemError << endl;
    return S_FALSE;
}

int MLCallbackTest::AskOverwrite(
                                 const wchar_t *existName, const time_t *existTime, const unsigned long long *existSize,
                                 const wchar_t *newName, const time_t *newTime, const unsigned long long *newSize,
                                 int *answer)
{
    *answer = lib7z::AskOverwrite::kYesToAll;
    return S_OK;
}

int MLCallbackTest::DecompressingItem(const wchar_t *name, bool isFolder, int mode, const unsigned long long *position)
{
    OUT << "[DecompressingItem] ";
    switch (mode)
    {
        case lib7z::DecompressingItemMode::kExtract:    OUT << "Extract "; break;
        case lib7z::DecompressingItemMode::kTest:       OUT << "Test "; break;
        case lib7z::DecompressingItemMode::kSkip:       OUT << "Skip "; break;
    }
    OUT << name << endl;
    return S_OK;
}

int MLCallbackTest::MessageError(const wchar_t *message)
{
    OUT << "[MessageError] " << message << "\n";
    return S_OK;
}

int MLCallbackTest::SetOperationResult(int operationResult, int kind)
{
    OUT << "[SetOperationResult] " << lib7z::GetExtractOperationErrorMessage(operationResult) << endl;
    return (operationResult==0)?S_OK:operationResult;
    
}

int MLCallbackTest::ThereAreNoFiles()
{
    return S_FALSE;
}

const wchar_t *MLCallbackTest::GetPassword()
{
    return (pass.empty()) ? NULL : pass.c_str();
}

void MLCallbackTest::setPassword(std::wstring newpass)
{
    pass = newpass;
}
