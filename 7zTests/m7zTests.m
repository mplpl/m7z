//
//  _zTests.m
//  7zTests
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "../7z/m7z.h"

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
-(BOOL)error:(NSString *)err {
    NSLog(@"%@", err);
    return NO;
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
@property (readonly) NSString *subDir;
@property (readonly) NSArray *subDirItems;
@property (readonly) NSString *subDirToUnpack;
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
    _subDir = @"/tmp/a";
    _subDirItems = @[@"/tmp/a/1.txt", @"/tmp/a/2.txt", @"/tmp/a/3.txt"];
    _subDirToUnpack = @"a";
    
    NSError *err;
    for (NSString *item in self.items) {
        [@"1111111111111111111111111111111111111111" writeToFile:item
                                                      atomically:YES
                                                        encoding:NSUTF32LittleEndianStringEncoding
                                                           error:&err];
    }
    NSFileManager *fm = [NSFileManager defaultManager];
    [fm createDirectoryAtPath:_subDir withIntermediateDirectories:YES attributes:nil error:&err];
    for (NSString *item in self.subDirItems) {
        [@"1111111111111111111111111111111111111111" writeToFile:item
                                                      atomically:YES
                                                        encoding:NSUTF32LittleEndianStringEncoding
                                                           error:&err];
    }
    
    [fm removeItemAtPath:self.missingArchName error:&err];
    [fm removeItemAtPath:self.unpackDir error:&err];
    
    _delegate = [[frmDelegate alloc] init];
}

- (void)tearDown {
    NSFileManager *fm = [NSFileManager defaultManager];
    NSError *err;
    for (NSString *item in self.items) {
        [fm removeItemAtPath:item error:&err];
    }
    for (NSString *item in self.subDirItems) {
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

- (void)testCompressDir {
    Archive *archive = [[Archive alloc] initWithName:self.archName];
    archive.delegate = self.delegate;
    XCTAssert([archive compressItem:@[self.subDir]] == 0);
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

- (void)testDecompressDir {
    Archive *archive = [[Archive alloc] initWithName:self.archName];
    XCTAssert([archive compressItem:@[self.subDir]] == 0);
    archive.delegate = self.delegate;
    
    XCTAssert([archive decompressItems:@[_subDirToUnpack] toDir:self.unpackDir] == 0);
    NSFileManager *fm = [NSFileManager defaultManager];
    NSError *err;
    NSArray *ret = [fm contentsOfDirectoryAtPath:[NSString stringWithFormat:@"%@/%@", self.unpackDir, _subDirToUnpack] error:&err];
    XCTAssert(ret.count == _subDirItems.count);
}

- (void)testDelete {
    Archive *archive = [[Archive alloc] initWithName:self.archName];
    XCTAssert([archive compressItem:self.items] == 0);
    archive.delegate = self.delegate;
    
    XCTAssert([archive deleteItems:@[_itemsToUnpack[0]]] == 0);
    
    XCTAssert([archive decompressToDir:self.unpackDir] == 0);
    NSFileManager *fm = [NSFileManager defaultManager];
    NSError *err;
    NSArray *ret = [fm contentsOfDirectoryAtPath:self.unpackDir error:&err];
    XCTAssert(ret.count == _items.count - 1);
}

@end
