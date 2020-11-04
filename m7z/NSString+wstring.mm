//
//  NSString+wstring.m
//  m7z
//
//  Created by MPL on 21/03/16.
//  Copyright © 2016-2020 MPL. All rights reserved.
//

#import "NSString+wstring.h"

@implementation NSString (wstring)

+stringWithWstring:(std::wstring)name {
    NSString *ret = [[NSString alloc] initWithBytes:name.data()
                                             length:name.size() * sizeof(wchar_t)
                                           encoding:NSUTF32LittleEndianStringEncoding];
    return ret;
}

-(std::wstring)wstring {

    NSData* data = [self.precomposedStringWithCanonicalMapping dataUsingEncoding:NSUTF32LittleEndianStringEncoding];
    std::wstring ws = std::wstring((wchar_t*)[data bytes], data.length / sizeof(wchar_t));
    return ws;
}

@end
