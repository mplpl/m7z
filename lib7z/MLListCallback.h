/*!
    MLListCallback.h
 
    Callback interface for list operation.
 */
// Copyright @ 2016-2020 MPL. All rights reserved.

#ifndef MLListCallback_h
#define MLListCallback_h

#import <string>

namespace lib7z
{
    
class MLListCallback
{
public:
    
    /*!
        Asks for password needed to complete the operation.
     
        \return password
     */
    virtual const std::wstring GetPassword() = 0;
};

}

#endif /* MLListCallback_h */
