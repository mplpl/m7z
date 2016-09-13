//
//  NSString+wstring.m
//  m7z
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
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

    return (const wchar_t *)[self cStringUsingEncoding:NSUTF32StringEncoding];
}

-(const wchar_t *)wcstr {
    
    return (const wchar_t *)[self cStringUsingEncoding:NSUTF32StringEncoding];
}

@end
