//
//  M7ZItem.h
//  m7z
//
//  Created by MPL on 21/03/16.
//  Copyright © 2016-2021 MPL. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface M7ZItem : NSObject

- (instancetype)initWithName:(NSString *)name
                        size:(unsigned long long)size
              sizeCompressed:(unsigned long long)sizeCompressed
                creationTime:(NSString *)creationTime
            modificationTime:(NSString *)modificationTime
                  accessTime:(NSString *)accessTime
                       attrs:(NSString *)attrs
                   encrypted:(BOOL)encrypted
           compressionMethod:(NSString *)compressionMethod;

@property (readonly) NSString *name;
@property (readonly) unsigned long long size;
@property (readonly) unsigned long long sizeCompressed;
@property (readonly) NSString *creationTime;
@property (readonly) NSString *modificationTime;
@property (readonly) NSString *accessTime;
@property (readonly) NSString *attrs;
@property (readonly) BOOL encrypted;
@property (readonly) NSString *compressionMethod;

@end
