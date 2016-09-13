//
//  Archive.h
//  7z
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
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
    M7Z_ORC_UnsupportedMethod = 1000,  //extract
    M7Z_ORC_DataError,                 //extract
    M7Z_ORC_CRCError,                  //extract
    M7Z_ORC_Unavailable,               //extract
    M7Z_ORC_UnexpectedEnd,             //extract
    M7Z_ORC_DataAfterEnd,              //extract
    M7Z_ORC_IsNotArc,                  //extract
    M7Z_ORC_HeadersError,              //extract
    M7Z_ORC_WrongPassword,             //extract
    M7Z_ORC_UpdateError        = 2000, //update
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

-(void)item:(NSString *)name mode:(int)mode;
-(BOOL)error:(NSString *)err resultCode:(int)resultCode;
-(int)shouldOverwriteItem:(NSString *)name date:(NSDate *)date size:(unsigned long long)size
                  newName:(NSString *)newName newDate:(NSDate *)newDate
                  newSize:(unsigned long long)newSize;
-(void)done;

@property (readonly) NSString *password;
@property unsigned long long total;
@property unsigned long long completed;
@property (readonly) BOOL shouldBreak;

@end


@interface M7ZArchive : NSObject

- (instancetype)initWithName:(NSString *)name;

-(int)listItemsTo:(NSMutableArray *)output;
-(int)addItems:(NSArray *)items;
-(int)addItems:(NSArray *)items encryptHeader:(BOOL)encryptHeader;
-(int)addItems:(NSArray *)items encryptHeader:(BOOL)encryptHeader compressionLevel:(NSInteger)compressionLevel;
-(int)extractAllToDir:(NSString *)dir;
-(int)extractItems:(NSArray *)items toDir:(NSString *)dir;
-(int)deleteItems:(NSArray *)items;
-(NSString *)errorForCode:(NSInteger)code;

@property (readonly) NSString *name;
@property (assign) id<M7ZArchiveDelegate> delegate;
@property NSString *workDir;

@end

