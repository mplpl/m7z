/*!
    UpdateCallback.h
    
    Callback interface for pack/compress operation.
*/
// Copyright @ 2016-2021 MPL. All rights reserved.

#ifndef __MLUPDATE_CALLBACK_H
#define __MLUPDATE_CALLBACK_H

#include "lib7z_types.h"
#include "MLListCallback.h"

namespace lib7z
{
    
namespace CompressingItemMode
{
    enum
    {
        kAdd = 0,
        kUpdate,
        kAnalyze,
        kReplicate,
        kRepack,
        kSkip,
        kDelete,
        kHeader
        
        // kNumDefined
    };
};
    
namespace UpdateOperationResult
{
    enum
    {
        kOK = 0
        , // kError
    };
}
    
/*!
    Interface used for callback for pack/compress operation.
 */
class MLUpdateCallback: public virtual MLListCallback
{
    
public:
    
    /*!
        Notifies about total number of bytes to be processed in order
        to complete requested opration.

        \param size total operation size in bytes
        \result 0 - continue operation, <>0 - break operation
     */
    virtual LIB7ZRC SetTotal(unsigned long long size) = 0;
    
    /*!
        Notifies about starting compression of items with given name.

        \param name item name
        \param isAnti item that will delete file or directory during extracting - this is special 7z
            feature usefull when making backup to store information about files that were deleted
        \param mode see CompressingItemMode
        \result 0 - continue operation, <>0 - break operation
     */
    virtual LIB7ZRC CompressingItem(const wchar_t *name, bool isAnti, int mode) = 0;
    
    /*!
        Notifiles about operation process (partial completion).

        \param completeValue completion progress in bytes (in context of size
        reported by SetTotal)
        \result 0 - continue operation, <>0 - break operation
     */
    virtual LIB7ZRC SetCompleted(const unsigned long long *completeValue) = 0;
    
    /*!
        Notifies about operation result.

        \param operationResult see UpdateOperationResult
        \param kind always '2' which indicated that operation is 'Update'
        \result 0 - continue operation, <>0 - break operation
         */
    virtual LIB7ZRC SetOperationResult(int operationResult, int kind) = 0;
    
    /*!
        Notifies about completion of the given operation.

        \result 0 - continue operation, <>0 - break operation
    */
    virtual LIB7ZRC FinishArchive() = 0;
 
    /*!
        Notifies about an error on opening a file
        \param name the name of file which cannot be opened
        \param systemError system error value
        \result 0 - skip & continue <>0 - break operation
     */
    virtual LIB7ZRC OpenFileError(const wchar_t *name, int systemError) = 0;
};

    
}
#endif
