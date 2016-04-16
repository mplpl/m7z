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
	const wchar_t *archDir = L"/Users/mpl/temp/1";

  wcout << "Listing\n";
	wcout << "*****************************\n";
	std::vector<DirectoryItem> retValue;
  result = MLListArchive(arch1, retValue);
  wcout << "*****************************\n";
	if (result != S_OK)
	  	wcout << "Error while listing archive (" << result << ":" << GetErrorMessage(result) << ")\n";

  printAll(retValue);
	MLCallbackTest ucallback;

	wcout << "\n\n\n";
  wcout << "Unpacking\n";
	wcout << "*****************************\n";
  result = MLDecompressArchive(arch1, archDir, ucallback);
	wcout << "*****************************\n";
	if (result != S_OK)
		wcout << "Error while extracting archive (" << result << ":" << GetErrorMessage(result) << ")\n";

	wcout << "\n\n\n";
  wcout << "Packing\n";
	wcout << "*****************************\n";
	vector<wstring> files;
	files.push_back(archDir);
	result = MLCompressArchive(arch2, files, ucallback);
	wcout << "*****************************\n";
	if (result != S_OK)
		wcout << "Error while compressing archive (" << result << ":" << GetErrorMessage(result) << ")\n";

	wcout << "\n\n\n";
  wcout << "Listing\n";
	wcout << "*****************************\n";
  retValue.clear();
  result = MLListArchive(arch2, retValue);
	wcout << "*****************************\n";
	if (result != S_OK)
		wcout << "Error while listing archive (" << result << ":" << GetErrorMessage(result) << ")\n";

  printAll(retValue);

  return 0;

}
