//
//  _zTests.m
//  7zTests
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "../7z/frm7z.h"

@interface frmDelegate : NSObject<ArchiveDelegate>
@end

@implementation frmDelegate

@synthesize total;
@synthesize completed;

-(void)item:(NSString *)name {
    NSLog(@"%@", name);
}
-(void)done {
    NSLog(@"Done:");
}
-(void)error:(NSString *)err {
    NSLog(@"%@", err);
}
-(NSString *)password {
    NSLog(@"Password asked");
    return @"";
}
@end

@interface frm7zTests : XCTestCase

@property (readonly) NSString *archName;
@property (readonly) NSString *missingArchName;
@property (readonly) NSArray *items;
@property (readonly) NSArray *itemsToUnpack;
@property (readonly) NSString *unpackDir;
@property (readonly) frmDelegate *delegate;
@end

@implementation frm7zTests

- (void)setUp {
    [super setUp];
    
    _archName = @"/tmp/1.7z";
    _missingArchName = @"/tmp/123.7z";
    _unpackDir = @"/tmp/1";
    _items = @[@"/tmp/1.txt", @"/tmp/2.txt", @"/tmp/3.txt"];
    _itemsToUnpack = @[@"2.txt", @"3.txt"];
    
    
    NSError *err;
    for (NSString *item in self.items) {
        [@"1111111111111111111111111111111111111111" writeToFile:item
                                                      atomically:YES
                                                        encoding:NSUTF32LittleEndianStringEncoding
                                                           error:&err];
    }
    
    NSFileManager *fm = [NSFileManager defaultManager];
    [fm removeItemAtPath:self.missingArchName error:&err];
    
    _delegate = [[frmDelegate alloc] init];
}

- (void)tearDown {
    NSFileManager *fm = [NSFileManager defaultManager];
    NSError *err;
    for (NSString *item in self.items) {
        [fm removeItemAtPath:item error:&err];
    }
    [fm removeItemAtPath:self.missingArchName error:&err];
    [fm removeItemAtPath:self.archName error:&err];
    [super tearDown];
}

- (void)testCompress {
    Archive *archive = [[Archive alloc] initWithName:self.archName];
    archive.delegate = self.delegate;
    XCTAssert([archive compressItem:self.items] == 0);
}

- (void)testList {
    Archive *archive = [[Archive alloc] initWithName:self.archName];
    XCTAssert([archive compressItem:self.items] == 0);
    
    NSArray *arr;
    XCTAssert((arr = [archive list]) != nil);
    for (Item *item in arr) {
        NSLog(@"%@ %d %d %@ %@", item.name, item.size, item.sizeCompressed, item.date, item.attrs);
    }
    
}

- (void)testListErr {
    Archive *archive = [[Archive alloc] initWithName:self.missingArchName];
    XCTAssert([archive list] == nil);
}

- (void)testDecompress {
    Archive *archive = [[Archive alloc] initWithName:self.archName];
    XCTAssert([archive compressItem:self.items] == 0);
    archive.delegate = self.delegate;
    XCTAssert([archive decompressToDir:self.unpackDir] == 0);
}

- (void)testDecompressSelected {
    Archive *archive = [[Archive alloc] initWithName:self.archName];
    XCTAssert([archive compressItem:self.items] == 0);
    archive.delegate = self.delegate;
    
    XCTAssert([archive decompressItems:_itemsToUnpack toDir:self.unpackDir] == 0);
}
@end
