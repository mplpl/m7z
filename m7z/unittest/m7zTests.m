//
//  _zTests.m
//  7zTests
//
//  Created by MPL on 21/03/16.
//  Copyright © 2016-2021 MPL. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "../m7z.h"

@interface frmDelegate : NSObject<M7ZArchiveDelegate>
@property int askedOverwrite;
@end

@implementation frmDelegate

@synthesize total;
@synthesize completed;
@synthesize shouldBreak;
@synthesize askedOverwrite;

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
- (BOOL)openFileError:(NSString *)name code:(int)code {
    NSLog(@"File %@ open error %d", name, code);
    return NO;
}
-(NSString *)password {
    NSLog(@"Password asked");
    return @"";
}

-(int)shouldOverwriteItem:(NSString *)name date:(NSDate *)date size:(unsigned long long)size
                  newName:(NSString *)newName newDate:(NSDate *)newDate newSize:(unsigned long long)newSize {
    
    askedOverwrite++;
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
@property (readonly) NSString *lockedDir;
@property (readonly) NSString *archNameZip;
@property (readonly) NSArray *itemsNls;
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
    _lockedDir = @"/tmp/b_locked";
    _archNameZip = @"/tmp/1.zip";
    _itemsNls = @[@"/tmp/ŻÓŁĆ.TXT"];
    
    
    NSError *err;
    for (NSString *item in self.items) {
        [@"1111111111111111111111111111111111111111" writeToFile:item
                                                      atomically:YES
                                                        encoding:NSUTF32LittleEndianStringEncoding
                                                           error:&err];
    }
    for (NSString *item in self.itemsNls) {
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
    
    [fm removeItemAtPath:_lockedDir error:&err];
    [fm createDirectoryAtPath:_lockedDir withIntermediateDirectories:YES attributes:nil error:&err];
    NSTask *task =[NSTask launchedTaskWithLaunchPath:@"/usr/bin/chflags" arguments:@[@"uchg", _lockedDir]];
    [task waitUntilExit];
    // NOTE: to check is a file is locked/immutable you can use 'ls -lO'
    
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

- (void)testDecompressSelectedDouble {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archName];
    XCTAssert([archive addItems:self.items] == 0);
    archive.delegate = self.delegate;
    XCTAssert([archive extractItems:_itemsToUnpack toDir:self.unpackDir] == 0);
    archive = [[M7ZArchive alloc] initWithName:self.archName];
    archive.delegate = self.delegate;
    self.delegate.askedOverwrite = 0;
    XCTAssert([archive extractItems:_itemsToUnpack toDir:self.unpackDir] == 0);
    XCTAssert(self.delegate.askedOverwrite == _itemsToUnpack.count);
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

/*
- (void)testConv {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:@"/Users/mpl/encoding_test_file/test.rar" encoding:@"CP852"];
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    XCTAssert(([archive listItemsTo:arr]) == 0);
    for (M7ZItem *s in arr) {
        NSLog(@"%@", s.name);
    }
    //unsigned ret = [archive addItems:@[@"/Users/mpl/ąęśćĄĘŚĆŁł_new.txt"]];
}
*/

-(void)testDecompressToLocked {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archName];
    XCTAssert([archive addItems:@[self.subDir]] == 0);
    archive.delegate = self.delegate;
    XCTAssert([archive extractItems:@[_subDirToUnpack] toDir:self.lockedDir] == M7Z_RC_FAIL);
}

-(void)testZipCompressWithEncoding {
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archNameZip encoding:@"CP852"];
    archive.delegate = self.delegate;
    XCTAssert([archive addItems:self.items] == 0);
}

-(void)testZipCompressNlsExisting {
    // tests adding a file with NLS in name to a ZIP in when it is there already
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archNameZip];
    archive.delegate = self.delegate;
    XCTAssert([archive addItems:self.itemsNls] == 0);
    XCTAssert([archive addItems:self.itemsNls] == 0);
}

-(void)testZipCompressNlsExistingDecomposed {
    // tests adding a file with NLS in name in decomposed Unicode form, to a ZIP in when it is there already
    M7ZArchive *archive = [[M7ZArchive alloc] initWithName:self.archNameZip];
    archive.delegate = self.delegate;
    NSMutableArray *arr = [[NSMutableArray alloc] init];
    for (NSString *item in self.itemsNls) {
        [arr addObject:item.decomposedStringWithCompatibilityMapping];
    }
    XCTAssert([archive addItems:arr] == 0);
    XCTAssert([archive addItems:arr] == 0);
}

@end
