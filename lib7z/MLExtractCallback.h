/*!
    MLExtractCallback.h
 
    Callback interface for extract/uncompress operation.
 */

#ifndef __MLEXTRACTCALLBACK_H
#define __MLEXTRACTCALLBACK_H

#include "lib7z_types.h"

namespace lib7z
{
/*!
    Enum used by AskOverwrite to decide what to do.
 */
namespace AskOverwrite
{
    enum
    {
        kYes,           /*!< overwrite this item */
        kYesToAll,      /*!< overwrite this and all next items*/
        kNo,            /*!< do NOT overwrite this item */
        kNoToAll,       /*!< do NOT overwrite this and all next items */
        kAutoRename,    /*!< ????? */
        kCancel         /*!< cancel the operation */
    };
}

/*!
    Enum used by operationResult variable of SetOperationResult callback.
 */
namespace ExtractOperationResult
{
    enum
    {
        kOK = 0,
        kUnsupportedMethod,
        kDataError,
        kCRCError,
        kUnavailable,
        kUnexpectedEnd,
        kDataAfterEnd,
        kIsNotArc,
        kHeadersError,
        kWrongPassword
    };
}

namespace DecompressingItemMode
{
    enum
    {
        kExtract = 0,
        kTest,
        kSkip
    };
}
/*!
    Interface used for callback for extract/uncompress operation.
 */
class MLExtractCallback
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
        Notifies about starting decompression of items with given name.
     
        \param name item name
        \param isFolder ?
        \param mode see DecompressingItemMode
        \param position ?
        \result 0 - continue operation, <>0 - break operation
     */
    virtual ORC DecompressingItem(const wchar_t *name, bool isFolder, int mode, const unsigned long long *position) = 0;
    
    /*!
        Notifiles about operation process (partial completion).
     
        \param completeValue completion progress in bytes (in context of size 
            reported by SetTotal)
        \result 0 - continue operation, <>0 - break operation
     */
    virtual ORC SetCompleted(const unsigned long long *completeValue) = 0;
    
    /*!
        Notifies about operation result.
     
        \param operationResult one of ExtractOperationResult
        \param kind always '1' which indicated that operation is 'Extract'
        \result 0 - continue operation, <>0 - break operation
     */
    virtual ORC SetOperationResult(int operationResult, int kind) = 0;
    
    /*!
        Notifies about completion of the given operation.
     
        \result 0 - continue operation, <>0 - break operation
     */
    virtual ORC FinishArchive() = 0;
    
    /*!
        Asks about decision whether or not overwrite existing file.
        
        \param existName existing item name
        \param existTime existing item modification time
        \param existSize existing item size
        \param newName new item name
        \param newTime new item modification time
        \param newSize new item size
        \param answer output variable used to determine decision - use one of AskOverwrite::Asnwer
        \result 0 - continue operation, <>0 - break operation
     */
    virtual ORC AskOverwrite(
         const wchar_t *existName, const time_t *existTime, const unsigned long long *existSize,
         const wchar_t *newName, const time_t *newTime, const unsigned long long *newSize,
         int *answer) = 0;
    
    /*!
        Asks for password needed to complete the operation.
     
        \return password
     */
    virtual const wchar_t *GetPassword() = 0;
    
    /*! no longer used - to be removed */
    virtual ORC ThereAreNoFiles() = 0;
    
    /*!
        Notifies about error occured during operation.
     
        \param message error message
        \result 0 - continue operation, <>0 - break operation
     */
    virtual ORC MessageError(const wchar_t *message) = 0;
};

    
}
#endif
