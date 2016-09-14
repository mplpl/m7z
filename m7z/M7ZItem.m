//
//  M7ZItem.m
//  m7z
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
//

#import "M7ZItem.h"

@implementation M7ZItem

- (instancetype)initWithName:(NSString *)name size:(int)size sizeCompressed:(int)sizeCompressed date:(NSString *)date attrs:(NSString *)attrs encrypted:(BOOL)encrypted {
    
    if (self = [super init]) {
        
        _name = name;
        _size = size;
        _sizeCompressed = sizeCompressed;
        _date = date;
        _attrs = attrs;
        _encrypted = encrypted;
    }
    
    return self;
    
}

@end
