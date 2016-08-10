// MLCallbackTest.cpp

#include "StdAfx.h"

#include "MLCallbackTest.h"

#include "Windows/Error.h"
#ifndef _7ZIP_ST
#include "Windows/Synchronization.h"
#endif

#ifndef _7ZIP_ST
static NWindows::NSynchronization::CCriticalSection g_CriticalSection;
#define MT_LOCK NWindows::NSynchronization::CCriticalSectionLock lock(g_CriticalSection);
#else
#define MT_LOCK
#endif

#include "7zip/Archive/IArchive.h"
#include "7zip/UI/Common/ArchiveExtractCallback.h"
#include "Windows/Error.h"
using namespace NWindows;

#include <iostream>
using namespace std;

int MLCallbackTest::SetTotal(unsigned long long size)
{
    wcout << "[SetTotal] " << size << endl;
    return S_OK;
}

int MLCallbackTest::CanNotFindError(const wchar_t *name, int systemError)
{
    wcout << "[CanNotFindError] Cannot find  " << name << " (error " << systemError << ")" << endl;
    return S_FALSE;
}

int MLCallbackTest::FinishArchive()
{
    wcout << "[FinishArchive] Done" << endl;
    return S_OK;
}

int MLCallbackTest::SetCompleted(const unsigned long long *completeValue)
{
    MT_LOCK
    wcout << "[SetCompleted] " << *completeValue << endl;
    //wcout << ",";
    wcout.flush();
    return S_OK;
}

int MLCallbackTest::CompressingItem(const wchar_t *name, bool isAnti)
{
    MT_LOCK
    if(isAnti)
        wcout << "[CompressingItem] Anti item    " << name << endl;
    else
        wcout << "[CompressingItem] Compressing  " << name << endl;
    return S_OK;
}

int MLCallbackTest::OpenFileError(const wchar_t *name, int systemError)
{
    MT_LOCK
    wcout << "[OpenFileError] " << name << " " << systemError << endl;
    return S_FALSE;
}

int MLCallbackTest::AskOverwrite(
                                 const wchar_t *existName, const time_t *existTime, const unsigned long long *existSize,
                                 const wchar_t *newName, const time_t *newTime, const unsigned long long *newSize,
                                 int *answer)
{
    *answer = NOverwriteAnswer::kYesToAll;
    return S_OK;
}

int MLCallbackTest::DecompressingItem(const wchar_t *name, bool isFolder, int askExtractMode, const unsigned long long *position)
{
    wcout << "[DecompressingItem] Unpacking " << name << endl;
    return S_OK;
}

int MLCallbackTest::MessageError(const wchar_t *message)
{
    wcout << "[MessageError] " << message << "\n";
    return S_OK;
}

int MLCallbackTest::SetOperationResult(int operationResult, int kind)
{
    switch(operationResult)
    {
        case NArchive::NExtract::NOperationResult::kOK:
            wcout << "[SetOperationResult] OK" << endl;
            break;
        case NArchive::NExtract::NOperationResult::kUnSupportedMethod:
            wcout << "[SetOperationResult] kUnSupportedMethod" << endl;
            break;
        case NArchive::NExtract::NOperationResult::kCRCError:
            wcout << "[SetOperationResult] kCRCError" << endl;
            break;
        case NArchive::NExtract::NOperationResult::kDataError:
            wcout << "[SetOperationResult] kDataError" << endl;
            break;
        case 1000 + NArchive::NExtract::NOperationResult::kCRCError:
            wcout << "[SetOperationResult] 1000 + kCRCError" << endl;
            break;
        case 1000 + NArchive::NExtract::NOperationResult::kDataError:
            wcout << "[SetOperationResult] 1000 + kDataError" << endl;
            break;
        default:
            wcout << "[SetOperationResult] UnknownError" << endl;
    }
    return (operationResult==0)?S_OK:operationResult;
    
}

int MLCallbackTest::ThereAreNoFiles()
{
    return S_FALSE;
}

const wchar_t *MLCallbackTest::GetPassword()
{
    return pass.c_str();
}

void MLCallbackTest::setPassword(std::wstring newpass)
{
    pass = newpass;
}