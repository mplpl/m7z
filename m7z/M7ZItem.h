//
//  M7ZItem.h
//  m7z
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface M7ZItem : NSObject

- (instancetype)initWithName:(NSString *)name size:(int)size sizeCompressed:(int)sizeCompressed date:(NSString *)date attrs:(NSString *)attrs encrypted:(BOOL)encrypted;

@property (readonly) NSString *name;
@property (readonly) int size;
@property (readonly) int sizeCompressed;
@property (readonly) NSString *date;
@property (readonly) NSString *attrs;
@property (readonly) BOOL encrypted;

@end
