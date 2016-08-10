#include <stdafx.h>
#include "lib7z.h"
#include <iostream>
#include <vector>

#include "MLCallbackTest.h"

using namespace std;

void printAll(vector<DirectoryItem> &retValue)
{
    for (vector<DirectoryItem>::iterator it = retValue.begin(); it < retValue.end(); it++) {
        wcout << it->name.c_str() << "    " << it->size << "     " << it->sizeCompressed << "     " << it->attrs.c_str() << "     " << it->date.c_str() << "\n";
    }
}


int main(int numArgs, const char *args[])
{
    HRESULT result;
    const wchar_t *arch1 = L"/Users/mpl/temp/1.7z";
    const wchar_t *arch2 = L"/Users/mpl/temp/2.7z";
    const wchar_t *arch3 = L"test.7z";
    const wchar_t *archDir = L"/Users/mpl/temp/1";
    
    wcout << "Listing\n";
    wcout << "*****************************\n";
    std::vector<DirectoryItem> retValue;
    result = MLListArchive(arch1, retValue);
    wcout << "*****************************\n";
    if (result != S_OK)
        wcout << "Error while listing archive (" << result << ":" << GetErrorMessage(result, 0) << ")\n";
    
    printAll(retValue);
    MLCallbackTest ucallback;
    
    wcout << "\n\n\n";
    /*wcout << "Unpacking\n";
     wcout << "*****************************\n";
     vector<wstring> files0;
     result = MLDecompressArchive(arch1, archDir, files0, ucallback);
     wcout << "*****************************\n";
     if (result != S_OK)
     wcout << "Error while extracting archive (" << result << ":" << GetErrorMessage(result) << ")\n";
     */
    wcout << "Unpacking selected\n";
    wcout << "*****************************\n";
    vector<wstring> files1;
    files1.push_back(L"Alloc.o");
    files1.push_back(L"7z.so");
    result = MLDecompressArchive(arch1, archDir, files1, ucallback);
    wcout << "*****************************\n";
    if (result != S_OK)
        wcout << "Error while extracting archive (" << result << ":" << GetErrorMessage(result, 1) << ")\n";
    
    wcout << "\n\n\n";
    wcout << "Packing\n";
    wcout << "*****************************\n";
    vector<wstring> files;
    files.push_back(archDir);
    result = MLCompressArchive(arch2, files, ucallback);
    wcout << "*****************************\n";
    if (result != S_OK)
        wcout << "Error while compressing archive (" << result << ":" << GetErrorMessage(result, 2) << ")\n";
    
    wcout << "\n\n\n";
    wcout << "Listing\n";
    wcout << "*****************************\n";
    retValue.clear();
    result = MLListArchive(arch2, retValue);
    wcout << "*****************************\n";
    if (result != S_OK)
        wcout << "Error while listing archive (" << result << ":" << GetErrorMessage(result, 0) << ")\n";
    
    printAll(retValue);
    
    wcout << "\n\n\n";
    wcout << "Unpacking encrypted with correct password\n";
    wcout << "*****************************\n";
    vector<wstring> files2;
    ucallback.setPassword(L"test");
    result = MLDecompressArchive(arch3, archDir, files2, ucallback);
    wcout << "*****************************\n";
    if (result != S_OK)
        wcout << "Error while extracting archive (" << result << ":" << GetErrorMessage(result, 1) << ")\n";
    
    wcout << "\n\n\n";
    wcout << "Unpacking encrypted with incorrect password\n";
    wcout << "*****************************\n";
    vector<wstring> files3;
    ucallback.setPassword(L"");
    result = MLDecompressArchive(arch3, archDir, files3, ucallback);
    wcout << "*****************************\n";
    if (result != S_OK)
        wcout << "Error while extracting archive (" << result << ":" << GetErrorMessage(result, 1) << ")\n";
    
    return 0;
    
}
