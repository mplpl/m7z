#ifndef __LIB7Z_H__
#define __LIB7Z_H__

#include <vector>
#include <string>
#include "MLUpdateCallback.h"
#include "MLExtractCallback.h"

class DirectoryItem
{
public:
	DirectoryItem(std::wstring _name, int _size, int _sizeCompressed, std::string _date, std::string _attrs):
		name(_name), size(_size), sizeCompressed(_sizeCompressed), date(_date), attrs(_attrs) {}

public:
		std::wstring name;
		int size;
		int sizeCompressed;
		std::string date;
		std::string attrs;
};

extern "C" int MLListArchive(std::wstring archiveName, std::vector<DirectoryItem> &retValue);
extern "C" int MLCompressArchive(std::wstring archiveName, std::vector<std::wstring> files, MLUpdateCallback &callback);
extern "C" int MLDecompressArchive(std::wstring archiveName, std::wstring outDir, std::vector<std::wstring> files, MLExtractCallback &callback);
extern "C" const wchar_t *GetErrorMessage(int result);

#endif
