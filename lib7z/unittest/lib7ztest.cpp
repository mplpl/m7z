#include <string>
#include "gtest/gtest.h"
#include "../lib7z.h"
#include "MLCallbackTest.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>

using namespace lib7z;

class lib7zTest : public ::testing::Test 
{
	protected:
		virtual void SetUp() 
		{
			generateTestFiles();
		}

		virtual void TearDown() 
		{
			deleteTestFiles();

		}


		std::wstring arch1 = L"/tmp/1.7z";
    	std::wstring arch2 = L"/tmp/2.7z";
    	std::wstring arch3 = L"/tmp/3.7z";
    
    	std::vector<std::wstring> testFiles = {L"/tmp/t1/1.tmp", L"/tmp/t1/2.tmp", L"/tmp/t1/3.tmp"};
		std::vector<std::wstring> testFiles2 = {L"t1/1.tmp", L"t1/2.tmp", L"t1/3.tmp"};

		std::wstring archDir1 = L"/tmp/t1";
    	std::wstring archDir2 = L"/tmp/t2";
    	std::wstring archDir3 = L"/tmp/t3";

    	void generateTestFiles()
		{
			std::string archDir1cs(archDir1.begin(), archDir1.end());
			mkdir(archDir1cs.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			
			std::string archDir2cs(archDir2.begin(), archDir2.end());
			mkdir(archDir1cs.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			
			std::string archDir3cs(archDir3.begin(), archDir3.end());
			mkdir(archDir1cs.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			
		    for (auto it = testFiles.begin(); it < testFiles.end(); it++) {
		        std::string fname( it->begin(), it->end());
		        std::ofstream fout(fname);
		        for (int i = 0; i < 300000; i++)
		        {
		            fout << "wferfjhefjkvhdfhivwerqepojvqs,c xsfvjsfhvqwr";
		        }
		        fout.close();
		    }
		}

		void deleteTestFiles() 
		{
			for (auto it = testFiles.begin(); it < testFiles.end(); it++) {
		        std::string fname(it->begin(), it->end());
		        remove(fname.c_str());
		    }
		    std::string arch1cs(arch1.begin(), arch1.end());
		    remove(arch1cs.c_str());

		    std::string arch2cs(arch2.begin(), arch2.end());
		    remove(arch2cs.c_str());

		    std::string arch3cs(arch3.begin(), arch3.end());
		    remove(arch3cs.c_str());

		    std::string archDir1cs(archDir1.begin(), archDir1.end());
			remove(archDir1cs.c_str());
			
			std::string archDir2cs(archDir2.begin(), archDir2.end());
			remove(archDir1cs.c_str());
			
			std::string archDir3cs(archDir3.begin(), archDir3.end());
			remove(archDir1cs.c_str());
		}

		void printAll(std::vector<DirectoryItem> &retValue)
		{
		    for (auto it = retValue.begin(); it < retValue.end(); it++) {
		        std::wcout << it->name.c_str() << "    " << it->size << "     " << it->sizeCompressed << "     " << it->attrs.c_str() << "     " << it->date.c_str() << "\n";
		    }
		}

		int list(std::wstring arch)
		{
			std::vector<DirectoryItem> retValue;
		    int result = MLListArchive(arch, retValue);
		    EXPECT_EQ(result, 0) << "Error while listing archive (" << result << ":" << GetErrorMessage(result) << ")";
		    //printAll(retValue);
		    return retValue.size();
		}

		MLCallbackTest ucallback;
	    std::vector<DirectoryItem> retValue;
};


TEST_F(lib7zTest, PackingAndList) 
{
	std::vector<std::wstring> files;
    files.push_back(archDir1);
    int result = MLCompressArchive(arch1, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << GetErrorMessage(result) << ")";

    ASSERT_EQ(list(arch1), 4) << "Incorrect number of items listes";
}

TEST_F(lib7zTest, PackingSelectedAndList) 
{
	std::vector<std::wstring> files;
    files.push_back(testFiles[0]);
    files.push_back(testFiles[2]);
    int result = MLCompressArchive(arch3, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << GetErrorMessage(result) << ")";
    
    ASSERT_EQ(list(arch3), 2) << "Incorrect number of items listes";
}

TEST_F(lib7zTest, UnpackSelected) 
{
	std::vector<std::wstring> files;
	files.push_back(archDir1);
	int result = MLCompressArchive(arch2, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << GetErrorMessage(result) << ")";
    
	files.clear();
	files.push_back(testFiles2[1]);
    files.push_back(testFiles2[2]);
    result = MLDecompressArchive(arch2, archDir2, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while extracting archive (" << result << ":" << GetErrorMessage(result) << ")";

}

TEST_F(lib7zTest, PackAndUnpackEncrypted) 
{
	std::vector<std::wstring> files;
    files.push_back(archDir1);
    ucallback.setPassword(L"test");
    int result = MLCompressArchive(arch2, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << GetErrorMessage(result) << ")";

    files.clear();
    ucallback.setPassword(L"test");
    result = MLDecompressArchive(arch2, archDir3, files, ucallback);
    EXPECT_EQ(result, 0) << "Error while extracting archive (" << result << ":" << GetErrorMessage(result) << ")";
    
    files.clear();
    ucallback.setPassword(L"");
    result = MLDecompressArchive(arch2, archDir3, files, ucallback);
    EXPECT_EQ(result, 1002) << "Incorrect return while extracting encrypted archive with wrong password("
    	 << result << ":" << GetErrorMessage(result) << ")\n";
}


