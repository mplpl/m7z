//
//  Archive.h
//  7z
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
//

#import <Foundation/Foundation.h>
 
@protocol ArchiveDelegate <NSObject>

-(void)item:(NSString *)name;

-(BOOL)error:(NSString *)err;
-(void)done;

@property (readonly) NSString *password;
@property unsigned long long total;
@property unsigned long long completed;
@property (readonly) BOOL shouldBreak;

@end


@interface Archive : NSObject

- (instancetype)initWithName:(NSString *)name;

@property (readonly) NSArray *list;
-(int)compressItem:(NSArray *)items;
-(int)compressItem:(NSArray *)items level:(NSInteger)compressionLevel;
-(int)decompressToDir:(NSString *)dir;
-(int)decompressItems:(NSArray *)items toDir:(NSString *)dir;
-(int)deleteItems:(NSArray *)items;
-(NSString *)errorForCode:(NSInteger)code kind:(NSInteger)kind;

@property (readonly) NSString *name;
@property (assign) id<ArchiveDelegate> delegate;
@property NSString *workDir;

@end

