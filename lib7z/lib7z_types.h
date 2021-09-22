//
//  lib7z_types.h
//  m7z
//
//  Created by MPL on 08/09/16.
//  Copyright © 2016-2021 MPL. All rights reserved.
//

/*!
    Basic types used by lib7z.
 */
#ifndef lib7z_types_h
#define lib7z_types_h

#include <string>

namespace lib7z
{


typedef int LIB7ZRC;    /*!< Return type for all function to perform operation on archives. */

/*! 
    Enum with defined LIB7ZRC values. In addition to values below, user
    defined codes can be returned by callbacks and that is propagated
    as return values from called lib7z function.
 */
namespace Lib7zReturnCode {
    
    enum {
        OK                  = 0x00000000L,
        FERROR              = 0x00000001L,
        NOTIMPL             = 0x80004001L,
        NOINTERFACE         = 0x80004002L,
        ABORT               = 0x80004004L,
        FAIL                = 0x80004005L,
        INVALIDFUNCTION     = 0x80030001L,
        OUTOFMEMORY         = 0x8007000EL,
        INVALIDARG          = 0x80070057L,
        CANTLOADCODECS      = 300001L,
        UOINITFAILED        = 300002L,
        CANTCREATEOUTDIR    = 300003L,
        CANTFINDARCHIVE     = 301001L
    };
}

/*!
    Class that describes a single item in archive.
    Used in output vector from MLListArchive function.
 */
class DirectoryItem
{
public:
    /*!
        Basic constructor that sets all the instance variables.
     */
    DirectoryItem(std::wstring _name, unsigned long long _size, unsigned long long _sizeCompressed,
                  std::string _creationTime, std::string _modificationTime, std::string _accessTime,
                  std::string _attrs, bool encrypted, std::wstring compressionMethod):
    name(_name), size(_size), sizeCompressed(_sizeCompressed), creationTime(_creationTime),
    modificationTime(_modificationTime), accessTime(_accessTime),
    attrs(_attrs), encrypted(encrypted), compressionMethod(compressionMethod) {}
    
public:
    
    std::wstring name;                  /*!< item path */
    unsigned long long size;            /*!< item size in bytes */
    unsigned long long sizeCompressed;  /*!< item compressed size in bytes */
    std::string creationTime;           /*!< item modification date */
    std::string modificationTime;       /*!< item modification date */
    std::string accessTime;             /*!< item modification date */
    std::string attrs;                  /*!< items attributes (in plain string) */
    bool encrypted;                     /*!< is this item encrypted */
    std::wstring compressionMethod;     /*!< item compression method */
};
    
}
#endif /* lib7z_types_h */
