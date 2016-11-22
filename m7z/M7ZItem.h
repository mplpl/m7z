//
//  M7ZItem.h
//  m7z
//
//  Created by MPL on 21/03/16.
//  Copyright © 2016 MPL. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface M7ZItem : NSObject

- (instancetype)initWithName:(NSString *)name size:(unsigned long long)size sizeCompressed:(unsigned long long)sizeCompressed
                        date:(NSString *)date attrs:(NSString *)attrs encrypted:(BOOL)encrypted;

@property (readonly) NSString *name;
@property (readonly) unsigned long long size;
@property (readonly) unsigned long long sizeCompressed;
@property (readonly) NSString *date;
@property (readonly) NSString *attrs;
@property (readonly) BOOL encrypted;

@end
