//
//  M7ZArchive.h
//  m7z
//
//  Created by MPL on 21/03/16.
//  Copyright © 2016-2021 MPL. All rights reserved.
//

#import <Foundation/Foundation.h>

enum M7ZReturnCode
{
    M7Z_RC_OK                  = 0x00000000L,
    M7Z_RC_FERROR              = 0x00000001L,
    M7Z_RC_NOTIMPL             = 0x80004001L,
    M7Z_RC_NOINTERFACE         = 0x80004002L,
    M7Z_RC_ABORT               = 0x80004004L,
    M7Z_RC_FAIL                = 0x80004005L,
    M7Z_RC_INVALIDFUNCTION     = 0x80030001L,
    M7Z_RC_OUTOFMEMORY         = 0x8007000EL,
    M7Z_RC_INVALIDARG          = 0x80070057L,
    M7Z_RC_CANTLOADCODECS      = 300001L,
    M7Z_RC_UOINITFAILED        = 300002L,
    M7Z_RC_CANTCREATEOUTDIR    = 300003L,
    M7Z_RC_CANTFINDARCHIVE     = 301001L
};

enum M7ZOperationResultCode
{
    M7Z_ORC_OK                = 0,
    M7Z_ORC_UnsupportedMethod = 1001,                           //extract
    M7Z_ORC_DataError,                                          //extract
    M7Z_ORC_CRCError,                                           //extract
    M7Z_ORC_Unavailable,                                        //extract
    M7Z_ORC_UnexpectedEnd,                                      //extract
    M7Z_ORC_DataAfterEnd,                                       //extract
    M7Z_ORC_IsNotArc,                                           //extract
    M7Z_ORC_HeadersError,                                       //extract
    M7Z_ORC_WrongPassword,                                      //extract
    M7Z_ORC_DataErrorPassword = M7Z_ORC_DataError + 100,        //extract
    M7Z_ORC_CRCErrorPassword  = M7Z_ORC_CRCError + 100,         //extract

    M7Z_ORC_UpdateError        = 2001,                          //update
    M7Z_ORC_OtherError         = 3000
};

enum M7ZItemMode
{
    M7Z_IM_Add                = 1000, //extract
    M7Z_IM_Update,                    //extract
    M7Z_IM_Analyze,                   //extract
    M7Z_IM_Replicate,                 //extract
    M7Z_IM_Repack,                    //extract
    M7Z_IM_ExtractSkip,               //extract
    M7Z_IM_Delete,                    //extract
    M7Z_IM_Header,                    //extract
    M7Z_IM_Extract            = 2000, //update
    M7Z_IM_Test,                      //update
    M7Z_IM_UpdateSkip                 //update
};

enum M7ZAskOverwrite
{
    M7Z_AO_Yes,           /*!< overwrite this item */
    M7Z_AO_YesToAll,      /*!< overwrite this and all next items*/
    M7Z_AO_No,            /*!< do NOT overwrite this item */
    M7Z_AO_NoToAll,       /*!< do NOT overwrite this and all next items */
    M7Z_AO_AutoRename,    /*!< ????? */
    M7Z_AO_Cancel         /*!< cancel the operation */
};

@protocol M7ZArchiveDelegate <NSObject>

/**
    Called when operation processing starts for a new item (file or directory).
    @param name name of item (file or directory)
    @param mode type of operation that will be executed on the item - one of M7ZItemMode
 */
-(void)item:(NSString *)name mode:(int)mode;

/**
    Called on error
    @param operationResultCode error that occured - one of M7ZOperationResultCode
    @result YES - continue, NO - abort
 */
-(BOOL)error:(int)operationResultCode;

/**
    Called on file open error
    @param name name of file that cannot be opened
    @param code system error code
    @result YES - continue, NO - abort
 */
-(BOOL)openFileError:(NSString *)name code:(int)code;

/**
    Called when a file need to be overwritted to ask the user what to do.
    @param name exsting file name
    @param date existing file modification date
    @param size existing file size
    @param newName new file name
    @param newDate new file modification date
    @param newSize new file size
    @return decision what to do - one of M7ZAskOverwrite
 */
-(int)shouldOverwriteItem:(NSString *)name date:(NSDate *)date size:(unsigned long long)size
                  newName:(NSString *)newName newDate:(NSDate *)newDate
                  newSize:(unsigned long long)newSize;
-(void)done;

/**
    Password to be used during extract/update operation.
 */
@property (readonly) NSString *password;

/**
    Total about of bytes that need to be handled to complete the operation.
 */
@property unsigned long long total;

/**
    Number of bytes already handled in order to complete the operation.
 */
@property unsigned long long completed;

/**
    Should on-going operation be interrupted as soon as possible.
 */
@property (readonly) BOOL shouldBreak;

@end


@interface M7ZArchive : NSObject

-(instancetype)initWithName:(NSString *)name;
-(instancetype)initWithName:(NSString *)name encoding:(NSString *)encoding;

-(int)listItemsTo:(NSMutableArray *)output;
-(int)addItems:(NSArray *)items;
-(int)addItems:(NSArray *)items encryptHeader:(BOOL)encryptHeader;
-(int)addItems:(NSArray *)items encryptHeader:(BOOL)encryptHeader compressionLevel:(NSInteger)compressionLevel;
-(int)extractAllToDir:(NSString *)dir;
-(int)extractItems:(NSArray *)items toDir:(NSString *)dir;
-(int)deleteItems:(NSArray *)items;

@property (readonly) NSString *name;
@property (readonly) NSString *encoding;
@property (assign) id<M7ZArchiveDelegate> delegate;
@property NSString *workDir;

+(NSArray<NSString *> *)supportedEncodings;

@end

