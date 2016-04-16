//
//  Item.m
//  7z
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
//

#import "Item.h"

@implementation Item

-initWithName:(NSString *)name size:(int)size sizeCompressed:(int)sizeCompressed date:(NSString *)date attrs:(NSString *)attrs {
    
    if (self = [super init]) {
        
        _name = name;
        _size = size;
        _sizeCompressed = sizeCompressed;
        _date = date;
        _attrs = attrs;
    }
    
    return self;
    
}

@end
