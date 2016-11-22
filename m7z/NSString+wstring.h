//
//  NSString+wstring.h
//  m7z
//
//  Created by MPL on 21/03/16.
//  Copyright © 2016 MPL. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <string>

@interface NSString (wstring)

+stringWithWstring:(std::wstring)name;

-(std::wstring)wstring;

-(const wchar_t*)wcstr;

@end
