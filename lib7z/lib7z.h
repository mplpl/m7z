/*!
    C++ iterface of lib7z library
 */
// Copyright @ 2016 MPL. All rights reserved.

#ifndef __LIB7Z_H__
#define __LIB7Z_H__

#include <vector>
#include <string>
#include "lib7z_types.h"
#include "MLUpdateCallback.h"
#include "MLExtractCallback.h"
#include "MLListCallback.h"

namespace lib7z
{
    
/*!
    List content of given archive file.
 
    \param archiveName path to archive file
    \retValue output variable used to return content of archive
    \return 0 = OK, <>0 = error - one of standard errors (see Lib7zReturnCode) or user defined
        error returned from callback function.
 */
extern LIB7ZRC MLListArchive(std::wstring archiveName, std::vector<DirectoryItem> &retValue,
                         MLListCallback &callback);

/*!
    Compress given set of files and/or directories into archive file.
    If archive file with given name exists it will be updated, otherwise a new
    file will be created.
 
    \param archiveName path to archive file
    \param files list of items to include - if any of item is a directory it will be
        included with its content, items that cannot be found will be ignored
    \param callback callback object used to communicate progress and errors
    \param compressionLevel compression level - number between 0 and 9 - 0 means 'no compression'
        and 9 is the highest compression
    \param workDir directory where temporary files will be created - blank mean 'current directory'
    \param encryptHeader should archive header be encrypted (that encrypt file names)
    \return 0 = OK, <>0 = error - one of standard errors (see Lib7zReturnCode) or user defined
        error returned from callback function.
 */
extern LIB7ZRC MLAddToArchive(std::wstring archiveName, std::vector<std::wstring> files,
                             MLUpdateCallback &callback, bool encryptHeader = false,
                             int compressionLevel = 9, std::wstring workDir = L"");

/*!
    Extract selected files from archive or all the files and store them in given
    directory. 'Callback' should be used to state what to do when some files are 
    already in the destination directory.
 
    \param archiveName path to archive file
    \param outDir output directory where extracted files will be written
    \param files list of items to extract - if an item is directory its content will be
        extracted as well, if an item is not in archive it will be ignores, if the list
        is empty, entire archive will be unpacked
    \param callback callback object used to communicate progress and errors
    \param workDir directory where temporary files will be created - blank mean 'current directory'
    \return 0 = OK, <>0 = error - one of standard errors (see Lib7zReturnCode) or user defined
        error returned from callback function.
 */
extern LIB7ZRC MLExtractFromArchive(std::wstring archiveName, std::wstring outDir, std::vector<std::wstring> files,
                                     MLExtractCallback &callback, std::wstring workDir = L"");

/*!
    Delete items from archive.
 
    \param archiveName path to archive file
    \param files items to be deleted from archive - if an item is not in archive it will be ignored
    \param callback object used to communicate progress and errors
    \param workDir directory where temporary files will be created - blank mean 'current directory'
    \return 0 = OK, <>0 = error - one of standard errors (see Lib7zReturnCode) or user defined
        error returned from callback function.
 */
extern LIB7ZRC MLDeleteFromArchive(std::wstring archiveName, std::vector<std::wstring> files,
                                     MLUpdateCallback &callback, std::wstring workDir = L"");

}

#endif
