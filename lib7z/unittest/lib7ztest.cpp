// Copyright @ 2016-2022 MPL. All rights reserved.

#include <string>
#include "gtest/gtest.h"
#include "../lib7z.h"
#include "MLCallbackTest.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>

using namespace lib7z;

class lib7zTest : public ::testing::Test 
{
    protected:

		std::wstring arch1 = L"/tmp/1.7z";
    	std::wstring arch2 = L"/tmp/2.7z";
    	std::wstring arch3 = L"/tmp/3.7z";
        std::wstring arch4 = L"/tmp/4.bz2";
    
        std::vector<std::wstring> testFiles = {L"/tmp/t1/1.tmp", L"/tmp/t1/2.tmp", L"/tmp/t1/3.tmp"};
        std::vector<std::wstring> testFiles2 ={L"t1/1.tmp", L"t1/2.tmp", L"t1/3.tmp"};
    
        std::wstring archDir1 = L"/tmp/t1";
    	std::wstring archDir2 = L"/tmp/t2";
    	std::wstring archDir3 = L"/tmp/t3";

        virtual void SetUp()
        {
            //testFiles.push_back(L"/tmp/t1/1.tmp");
            //testFiles.push_back(L"/tmp/t1/2.tmp");
            //testFiles.push_back(L"/tmp/t1/3.tmp");
            
            //testFiles2.push_back(L"t1/1.tmp");
            //testFiles2.push_back(L"t1/2.tmp");
            //testFiles2.push_back(L"t1/3.tmp");
                                
            generateTestFiles();
        }
        
        virtual void TearDown() 
        {
            deleteTestFiles();
        }
                            
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
		        std::ofstream fout(fname.c_str());
		        for (int i = 0; i < 10000; i++)
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
            
            std::string arch4cs(arch4.begin(), arch4.end());
            remove(arch4cs.c_str());

		    std::string archDir1cs(archDir1.begin(), archDir1.end());
            std::ostringstream os1;
            os1 << "rm -rf " << archDir1cs;
            system(os1.str().c_str());
			
            std::string archDir2cs(archDir2.begin(), archDir2.end());
            std::ostringstream os2;
            os2 << "rm -rf " << archDir2cs;
            system(os2.str().c_str());
			
			std::string archDir3cs(archDir3.begin(), archDir3.end());
            std::ostringstream os3;
            os3 << "rm -rf " << archDir3cs;
            system(os3.str().c_str());
		}

		void printAll(std::vector<DirectoryItem> &retValue)
		{
		    for (auto it = retValue.begin(); it < retValue.end(); it++) {
		        std::wcout << it->name.c_str()
                    << "    " << it->size
                    << "     " << it->sizeCompressed
                    << "     " << it->attrs.c_str()
                    << "     " << it->creationTime.c_str()
                    << "     " << it->modificationTime.c_str()
                    << "     " << it->accessTime.c_str()
                    << "\n";
		    }
		}

		int list(std::wstring arch)
		{
			std::vector<DirectoryItem> retValue;
		    int result = MLListArchive(arch, retValue, ucallback);
		    EXPECT_EQ(result, 0) << "Error while listing archive (" << result << ":" << result << ")";
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
    int result = MLAddToArchive(arch1, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << result << ")";

    ASSERT_EQ(list(arch1), 4) << "Incorrect number of items listes";
}

TEST_F(lib7zTest, PackingSelectedAndList) 
{
	std::vector<std::wstring> files;
    files.push_back(testFiles[0]);
    files.push_back(testFiles[2]);
    int result = MLAddToArchive(arch3, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << result << ")";
    
    ASSERT_EQ(list(arch3), 2) << "Incorrect number of items listes";
}

TEST_F(lib7zTest, ExtractSelected)
{
	std::vector<std::wstring> files;
	files.push_back(archDir1);
	int result = MLAddToArchive(arch2, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << result << ")";
    
	files.clear();
	files.push_back(testFiles2[1]);
    files.push_back(testFiles2[2]);
    result = MLExtractFromArchive(arch2, archDir2, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while extracting archive (" << result << ":" << result << ")";

}

TEST_F(lib7zTest, PackAndExtractEncrypted)
{
	std::vector<std::wstring> files;
    files.push_back(archDir1);
    ucallback.setPassword(L"test");
    int result = MLAddToArchive(arch2, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << result << ")";

    files.clear();
    ucallback.setPassword(L"test");
    result = MLExtractFromArchive(arch2, archDir3, files, ucallback);
    EXPECT_EQ(result, 0) << "Error while extracting archive (" << result << ":" << result << ")";
    
    files.clear();
    ucallback.setPassword(L"");
    result = MLExtractFromArchive(arch2, archDir3, files, ucallback);
    EXPECT_EQ(result, ExtractOperationResult::kDataErrorPassword) << "Incorrect return while extracting encrypted archive with wrong password("
    	 << result << ":" << result << ")\n";
}

TEST_F(lib7zTest, PackAndListWithHeaderEncrypted)
{
    std::vector<std::wstring> files;
    files.push_back(archDir1);
    ucallback.setPassword(L"test");
    int result = MLAddToArchive(arch2, files, ucallback, true);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << result << ")";

    ASSERT_EQ(list(arch2), 4) << "Incorrect number of items listes";
    
    // try wrong password
    ucallback.setPassword(L"testXXX");
    std::vector<DirectoryItem> retValue;
    int lresult = MLListArchive(arch2, retValue, ucallback);
    ASSERT_EQ(lresult, 1) << "Encrypted archive listed even with wrong password";
}

TEST_F(lib7zTest, DoubleExtract)
{
    std::vector<std::wstring> files;
    files.push_back(testFiles[1]);
    int result = MLAddToArchive(arch2, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << result << ")";
    
    files.clear();
    result = MLExtractFromArchive(arch2, archDir3, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while extracting archive (" << result << ":" << result << ")";
    
    files.clear();
    ucallback.askOverwriteCounter = 0;
    result = MLExtractFromArchive(arch2, archDir3, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while extracting archive (" << result << ":" << result << ")";
    
    ASSERT_EQ(ucallback.askOverwriteCounter, 1) << "AskOverwrite called incorrect number of times";

}

TEST_F(lib7zTest, Bz2Test)
{
    // I found an issue with older version of m7z about EXC_BAD_ACCESS during extract
    std::vector<std::wstring> files;
    files.push_back(testFiles[1]);
    int result = MLAddToArchive(arch4, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing bz2 archive (" << result << ":" << result << ")";
    
    files.clear();
    result = MLExtractFromArchive(arch4, archDir3, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while extracting archive (" << result << ":" << result << ")";
}

TEST_F(lib7zTest, RenameTest)
{
    std::vector<std::wstring> files;
    files.push_back(testFiles[0]);
    int result = MLAddToArchive(arch2, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << result << ")";
    
    files.clear();
    files.push_back(L"1.tmp");
    files.push_back(L"1.tmp.new");
    result = MLRenameItemsInArchive(arch2, files, ucallback);
    ASSERT_EQ(result, 0) << "Error while renaming item in archive (" << result << ":" << result << ")";
    
    std::vector<DirectoryItem> retValue;
    result = MLListArchive(arch2, retValue, ucallback);
    EXPECT_EQ(result, 0) << "Error while listing archive (" << result << ":" << result << ")";
    
    ASSERT_EQ(retValue.size(), 1) << "Wrong number of items in archive";
    ASSERT_EQ(retValue[0].name, L"1.tmp.new") << "Item has not been renamed";
}

TEST_F(lib7zTest, AddWithExclusionsWildcards)
{
    std::vector<std::wstring> files;
    files.push_back(testFiles[0]);
    files.push_back(testFiles[1]);
    std::vector<std::wstring> exclusionWildcards;
    exclusionWildcards.push_back(L"2.*");
    int result = MLAddToArchive(arch2, files, ucallback, false, 1, L"", L"", false, false, exclusionWildcards);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << result << ")";
    
    std::vector<DirectoryItem> retValue;
    result = MLListArchive(arch2, retValue, ucallback);
    EXPECT_EQ(result, 0) << "Error while listing archive (" << result << ":" << result << ")";
    
    ASSERT_EQ(retValue.size(), 1) << "Wrong number of items in archive";
    ASSERT_EQ(retValue[0].name, L"1.tmp") << "Item has not been excluded";
}

TEST_F(lib7zTest, ExtractWithExclusionsWildcards)
{
    int result = MLAddToArchive(arch2, testFiles, ucallback);
    ASSERT_EQ(result, 0) << "Error while compressing archive (" << result << ":" << result << ")";
    
    std::vector<std::wstring> exclusionWildcards;
    exclusionWildcards.push_back(L"2.*");
    
    std::vector<std::wstring> files;
    result = MLExtractFromArchive(arch2, archDir3, files, ucallback, L"", L"", exclusionWildcards);
    ASSERT_EQ(result, 0) << "Error while extracting archive (" << result << ":" << result << ")";
    
    std::string archDir3C(archDir3.begin(), archDir3.end());
    DIR *dir = opendir(archDir3C.c_str());
    ASSERT_EQ(dir != NULL, true);
    
    struct dirent *entry;
    int count = 0;
    bool found = false;
    while ((entry = readdir(dir)) != NULL) {
        std::string s(entry->d_name);
        if (s == std::string("..") || s == std::string(".")) continue;
        if (s == std::string("2.tmp")) found = true;
        count++;
    }
    closedir(dir);
    
    ASSERT_EQ(found, false) << "2.tmp found";
    ASSERT_EQ(count, 2) << "Item not filtered";
}
