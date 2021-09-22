//
//  M7ZItem.m
//  m7z
//
//  Created by MPL on 21/03/16.
//  Copyright © 2016-2021 MPL. All rights reserved.
//

#import "M7ZItem.h"

@implementation M7ZItem

- (instancetype)initWithName:(NSString *)name
                        size:(unsigned long long)size
              sizeCompressed:(unsigned long long)sizeCompressed
                creationTime:(NSString *)creationTime
            modificationTime:(NSString *)modificationTime
                  accessTime:(NSString *)accessTime
                       attrs:(NSString *)attrs
                   encrypted:(BOOL)encrypted
           compressionMethod:(NSString *)compressionMethod {
    
    if (self = [super init]) {
        
        _name = name;
        _size = size;
        _sizeCompressed = sizeCompressed;
        _creationTime = creationTime;
        _modificationTime = modificationTime;
        _accessTime = accessTime;
        _attrs = attrs;
        _encrypted = encrypted;
        _compressionMethod = compressionMethod;
    }
    
    return self;
    
}

@end
