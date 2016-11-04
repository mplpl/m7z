//
//  _zTests.m
//  7zTests
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "../m7z.h"

@interface frmDelegate : NSObject<M7ZArchiveDelegate>
@end

@implementation frmDelegate

@synthesize total;
@synthesize completed;
@synthesize shouldBreak;

-(void)item:(NSString *)name mode:(int)mode {
    NSLog(@"%@ mode %d", name, mode);
}
-(void)done {
    NSLog(@"Done:");
}
-(BOOL)error:(int)operationResultCode {
    NSLog(@"Error %d", operationResultCode);
    return NO;
}
-(NSString *)password {
    NSLog(@"Password asked");
    return @"";
}

-(int)shouldOverwriteItem:(NSString *)name date:(NSDate *)date size:(unsigned long long)size
                  newName:(NSString *)newName newDate:(NSDate *)newDate newSize:(unsigned long long)newSize {
    
    return M7Z_AO_Yes;
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
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archName];
    archive.delegate = self.delegate;
    XCTAssert([archive addItems:self.items] == 0);
}

- (void)testCompressDir {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archName];
    archive.delegate = self.delegate;
    XCTAssert([archive addItems:@[self.subDir]] == 0);
}

- (void)testList {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archName];
    XCTAssert([archive addItems:self.items] == 0);
    
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    XCTAssert(([archive listItemsTo:arr]) == 0);
    for (M7ZItem *item in arr) {
        NSLog(@"%@ %llu %llu %@ %@", item.name, item.size, item.sizeCompressed, item.date, item.attrs);
    }
    
}

- (void)testListErr {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.missingArchName];
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    XCTAssert([archive listItemsTo:arr] != 0);
}

- (void)testDecompress {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archName];
    XCTAssert([archive addItems:self.items] == 0);
    archive.delegate = self.delegate;
    XCTAssert([archive extractAllToDir:self.unpackDir] == 0);
}

- (void)testDecompressSelected {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archName];
    XCTAssert([archive addItems:self.items] == 0);
    archive.delegate = self.delegate;
    
    XCTAssert([archive extractItems:_itemsToUnpack toDir:self.unpackDir] == 0);
}

- (void)testDecompressDir {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archName];
    XCTAssert([archive addItems:@[self.subDir]] == 0);
    archive.delegate = self.delegate;
    
    XCTAssert([archive extractItems:@[_subDirToUnpack] toDir:self.unpackDir] == 0);
    NSFileManager *fm = [NSFileManager defaultManager];
    NSError *err;
    NSArray *ret = [fm contentsOfDirectoryAtPath:[NSString stringWithFormat:@"%@/%@", self.unpackDir, _subDirToUnpack] error:&err];
    XCTAssert(ret.count == _subDirItems.count);
}

- (void)testDelete {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archName];
    XCTAssert([archive addItems:self.items] == 0);
    archive.delegate = self.delegate;
    
    XCTAssert([archive deleteItems:@[_itemsToUnpack[0]]] == 0);
    
    XCTAssert([archive extractAllToDir:self.unpackDir] == 0);
    NSFileManager *fm = [NSFileManager defaultManager];
    NSError *err;
    NSArray *ret = [fm contentsOfDirectoryAtPath:self.unpackDir error:&err];
    XCTAssert(ret.count == _items.count - 1);
}

@end
