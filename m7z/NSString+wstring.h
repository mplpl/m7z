//
//  NSString+wstring.h
//  m7z
//
//  Created by MPL on 21/03/16.
//  Copyright © 2016-2020 MPL. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <string>

@interface NSString (wstring)

+stringWithWstring:(std::wstring)ws;

-(std::wstring)wstring;

@end
