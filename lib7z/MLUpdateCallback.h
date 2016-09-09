/*!
    UpdateCallback.h
    
    Callback interface for pack/compress operation.
*/

#ifndef __MLUPDATE_CALLBACK_H
#define __MLUPDATE_CALLBACK_H

#include "lib7z_types.h"

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
class MLUpdateCallback
{
    
public:
    
    /*!
        Notifies about total number of bytes to be processed in order
        to complete requested opration.

        \param size total operation size in bytes
        \result 0 - continue operation, <>0 - break operation
     */
    virtual ORC SetTotal(unsigned long long size) = 0;
    
    /*!
        Notifies about starting compression of items with given name.

        \param name item name
        \param isAnti item that will delete file or directory during extracting - this is special 7z
            feature usefull when making backup to store information about files that were deleted
        \param mode see CompressingItemMode
        \result 0 - continue operation, <>0 - break operation
     */
    virtual ORC CompressingItem(const wchar_t *name, bool isAnti, int mode) = 0;
    
    /*!
        Notifiles about operation process (partial completion).

        \param completeValue completion progress in bytes (in context of size
        reported by SetTotal)
        \result 0 - continue operation, <>0 - break operation
     */
    virtual ORC SetCompleted(const unsigned long long *completeValue) = 0;
    
    /*!
        Notifies about operation result.

        \param operationResult see UpdateOperationResult
        \param kind always '2' which indicated that operation is 'Update'
        \result 0 - continue operation, <>0 - break operation
         */
    virtual ORC SetOperationResult(int operationResult, int kind) = 0;
    
    /*!
        Notifies about completion of the given operation.

        \result 0 - continue operation, <>0 - break operation
    */
    virtual ORC FinishArchive() = 0;
    
    /*!
        Asks for password needed to complete the operation. 
        If file should not be encrypter NULL must be returned.

        \return password or NULL if no encryption is needed
     */
    virtual const wchar_t *GetPassword() = 0;
    
    /*! to be removed */
    virtual ORC CanNotFindError(const wchar_t *name, int systemError) = 0;
    virtual ORC OpenFileError(const wchar_t *name, int systemError) = 0;
};

    
}
#endif
