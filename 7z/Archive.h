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

-(void)error:(NSString *)err;
-(void)done;

-(NSString *)password;

@property (readwrite) unsigned long long total;
@property (readwrite) unsigned long long completed;

@end


@interface Archive : NSObject

-initWithName:(NSString *)name;

-(NSArray *)list;
-(int)compressItem:(NSArray *)items;
-(int)decompressToDir:(NSString *)dir;
-(int)decompressItems:(NSArray *)items toDir:(NSString *)dir;


@property (readonly) NSString *name;
@property (assign) id<ArchiveDelegate> delegate;

@end

