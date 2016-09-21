#include <lib7z.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <sys/stat.h>

using namespace std;

std::ofstream devnull("/dev/null");
#define OUT wcout

using namespace lib7z;

class MLCallbackTest: public MLUpdateCallback, public MLExtractCallback
{
    
public:

    int SetTotal(unsigned long long size) 
    {
        OUT << "[SetTotal] " << size << endl;
        return 0;
    }

    int CompressingItem(const wchar_t *name, bool isAnti, int mode)
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
        return 0;
    }

    int DecompressingItem(const wchar_t *name, bool isFolder, int mode, const unsigned long long *position)
    {
        OUT << "[DecompressingItem] ";
        switch (mode)
        {
            case lib7z::DecompressingItemMode::kExtract:    OUT << "Extract "; break;
            case lib7z::DecompressingItemMode::kTest:       OUT << "Test "; break;
            case lib7z::DecompressingItemMode::kSkip:       OUT << "Skip "; break;
        }
        OUT << name << endl;
        return 0;
    }

    int SetCompleted(const unsigned long long *completeValue)
    {
        OUT << "[SetCompleted] " << *completeValue << endl;
        //wcout << ",";
        OUT.flush();
        return 0;
    }

    int SetOperationResult(int operationResult, int kind) 
    {
        OUT << "[SetOperationResult] " << operationResult << endl;
        return 0;
    }

    int FinishArchive() 
    {
        OUT << "[FinishArchive] Done" << endl;
        return 0;
    }
    
    int AskOverwrite(
                     const wchar_t *existName, const time_t *existTime, const unsigned long long *existSize,
                     const wchar_t *newName, const time_t *newTime, const unsigned long long *newSize,
                     int *answer) 
    {
        *answer = AskOverwrite::kYesToAll;
        return 0;
    }
       
    int CanNotFindError(const wchar_t *name, int systemError) 
    {
        OUT << "[CanNotFindError] Cannot find  " << name << " (error " << systemError << ")" << endl;
        return 1;
    }

    int OpenFileError(const wchar_t *name, int systemError) 
    {
        OUT << "[OpenFileError] " << name << " " << systemError << endl;
        return 1;
    }
    
    int MessageError(const wchar_t *message) 
    {
        OUT << "[MessageError] " << message << endl;
        return 0;
    }

    int ThereAreNoFiles() 
    {
        return 1;
    }
    
    const wchar_t *GetPassword() 
    {
        OUT << "[GetPassword]" << endl;
        return (pass.empty()) ? NULL : pass.c_str();

    }

    void setPassword(std::wstring newpass) 
    {
        pass = newpass;
    }
    
private:
    std::wstring pass;
};



void printAll(vector<DirectoryItem> &retValue)
{
    for (vector<DirectoryItem>::iterator it = retValue.begin(); it < retValue.end(); it++) {
        wcout << it->name.c_str() << "    " << it->size << "     " << it->sizeCompressed << "     "
            << it->attrs.c_str() << "     " << it->date.c_str()
            << "     " << ((it->encrypted)?"E":"") << "\n";
    }
}

void generateTestFiles(vector<wstring> testFiles, std::wstring archDir)
{
    std::string archDir1cs(archDir.begin(), archDir.end());
    mkdir(archDir1cs.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    for (vector<wstring>::iterator it = testFiles.begin(); it < testFiles.end(); it++) {
        std::string fname( it->begin(), it->end());
        ofstream fout(fname.c_str());
        for (int i = 0; i < 300000; i++)
        {
            fout << "wferfjhefjkvhdfhivwerqepojvqs,c xsfvjsfhvqwr";
        }
        fout.close();
    }
}

int main(int numArgs, const char *args[])
{
    int result;
    vector<wstring> files;
    MLCallbackTest ucallback;
    std::vector<DirectoryItem> retValue;
    
    const wchar_t *arch1 = L"/tmp/1.7z";
    const wchar_t *arch2 = L"/tmp/2.7z";
    const wchar_t *arch3 = L"/tmp/3.7z";
    
    vector<wstring> testFiles;
    testFiles.push_back(L"/tmp/t1/1.tmp");
    testFiles.push_back(L"/tmp/t1/2.tmp");
    testFiles.push_back(L"/tmp/t1/3.tmp");
    
    vector<wstring> testFiles2;
    testFiles2.push_back(L"t1/1.tmp");
    testFiles2.push_back(L"t1/2.tmp");
    testFiles2.push_back(L"t1/3.tmp");
    
    const wchar_t *archDir1 = L"/tmp/t1";
    const wchar_t *archDir2 = L"/tmp/t2";
    const wchar_t *archDir3 = L"/tmp/t3";
    
    generateTestFiles(testFiles, archDir1);

    wcout << "\n\n\n";
    wcout << "Packing directory\n";
    wcout << "*****************************\n";
    files.clear();
    files.push_back(archDir1);
    result = MLAddToArchive(arch1, files, ucallback);
    wcout << "*****************************\n";
    if (result)
        wcout << "Error while compressing archive (" << result << ":" << result << ")\n";
    
    wcout << "\n\n\n";
    wcout << "Listing\n";
    wcout << "*****************************\n";
    retValue.clear();
    result = MLListArchive(arch1, retValue, ucallback);
    wcout << "*****************************\n";
    if (result)
        wcout << "Error while listing archive (" << result << ":" << result << ")\n";
    
    printAll(retValue);
    
    wcout << "\n\n\n";
    wcout << "Packing selected\n";
    wcout << "*****************************\n";
    files.clear();
    files.push_back(testFiles[0]);
    files.push_back(testFiles[2]);
    result = MLAddToArchive(arch3, files, ucallback);
    wcout << "*****************************\n";
    if (result)
        wcout << "Error while compressing archive (" << result << ":" << result << ")\n";
    
    wcout << "\n\n\n";
    wcout << "Listing\n";
    wcout << "*****************************\n";
    retValue.clear();
    result = MLListArchive(arch3, retValue, ucallback);
    wcout << "*****************************\n";
    if (result)
        wcout << "Error while listing archive (" << result << ":" << result << ")\n";
    
    printAll(retValue);

    wcout << "\n\n\n";
    wcout << "Unpacking selected\n";
    wcout << "*****************************\n";
    files.clear();
    files.push_back(testFiles2[1]);
    files.push_back(testFiles2[2]);
    result = MLExtractFromArchive(arch1, archDir2, files, ucallback);
    wcout << "*****************************\n";
    if (result)
        wcout << "Error while extracting archive (" << result << ":" << result << ")\n";
    
    wcout << "\n\n\n";
    wcout << "Packing with password\n";
    wcout << "*****************************\n";
    files.clear();
    files.push_back(archDir1);
    ucallback.setPassword(L"test");
    result = MLAddToArchive(arch2, files, ucallback, true);
    wcout << "*****************************\n";
    if (result)
        wcout << "Error while compressing archive (" << result << ":" << result << ")\n";

    wcout << "\n\n\n";
    wcout << "Listing\n";
    wcout << "*****************************\n";
    retValue.clear();
    result = MLListArchive(arch2, retValue, ucallback);
    wcout << "*****************************\n";
    if (result)
        wcout << "Error while listing archive (" << result << ":" << result << ")\n";
    
    printAll(retValue);
    
    wcout << "\n\n\n";
    wcout << "Unpacking encrypted with password\n";
    wcout << "*****************************\n";
    files.clear();
    result = MLExtractFromArchive(arch2, archDir3, files, ucallback);
    wcout << "*****************************\n";
    if (result)
        wcout << "Error while extracting archive (" << result << ":" << result << ")\n";
    
    return 0;
    
}
