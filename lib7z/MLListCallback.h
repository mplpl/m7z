/*!
    MLListCallback.h
 
    Callback interface for list operation.
 */
// Copyright @ 2016 MPL. All rights reserved.

#ifndef MLListCallback_h
#define MLListCallback_h

namespace lib7z
{
    
class MLListCallback
{
public:
    
    /*!
        Asks for password needed to complete the operation.
     
        \return password
     */
    virtual const wchar_t *GetPassword() = 0;
};

}

#endif /* MLListCallback_h */
