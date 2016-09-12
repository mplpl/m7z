//
//  Archive.m
//  7z
//
//  Created by mlabenski on 21/03/16.
//  Copyright © 2016 mlabenski. All rights reserved.
//

#import "Archive.h"
#import "Item.h"
#import "NSString+wstring.h"

#import <string>
#import <vector>

#import "lib7z.h"
#import "MLUpdateCallback.h"
#import "MLExtractCallback.h"

using namespace lib7z;

class CallbackTest: public MLUpdateCallback, public MLExtractCallback
{
    
public:
    id<ArchiveDelegate> delegat;
    
    CallbackTest(id<ArchiveDelegate> delegat): delegat(delegat) {}
    
    int SetTotal(unsigned long long size) {
        delegat.total = size;
        return 0;
    }
    
    int CompressingItem(const wchar_t *name, bool isAnti, int mode){
        [delegat item:[NSString stringWithWstring:name]];
        return delegat.shouldBreak;
    }
    
    int DecompressingItem(const wchar_t *name, bool isFolder, int mode, const unsigned long long *position) {
        [delegat item:[NSString stringWithWstring:name]];
        return 0;
    }
    
    int SetCompleted(const UInt64 *completeValue) {
        delegat.completed = *completeValue;
        return delegat.shouldBreak;
    }
    
    int SetOperationResult(int x, int kind) {
        if (x != 0) {
            //if ([delegat error:[NSString stringWithWstring:GetErrorMessage(x, kind)]] == NO)
            if ([delegat error:[NSString stringWithWstring:GetExtractOperationErrorMessage(x)]] == NO)
            {
                return x;
            }
        }
        return 0;
    }
    
    int FinishArchive() {
        [delegat done];
        return 0;
    }
    
    int AskOverwrite (
        const wchar_t *existName, const time_t *existTime, const unsigned long long *existSize,
        const wchar_t *newName, const time_t *newTime, const unsigned long long *newSize,
        int *answer) {
        
        //TODO add to interface
        //NSLog(@"%@", @"TEST");
        *answer = AskOverwrite::kYesToAll;
        return 0;
    }
    
    const wchar_t *GetPassword() {
        
        NSString *pass = delegat.password;

        if (!pass) {
            return NULL;
        } else {
            return [pass wcstr];
        }
    }
    
    int PrepareOperation(const wchar_t *name, bool isFolder, int askExtractMode, const unsigned long long *position) {
        [delegat item:[NSString stringWithWstring:name]];
        return 0;
    }
    
    int OpenFileError(const wchar_t *name, int systemError) {
        [delegat error:[NSString stringWithFormat:@"Open file error %@", [NSString stringWithWstring:name]]];
        return 0;
    }
    
    int CanNotFindError(const wchar_t *name, int systemError) {
        [delegat error:[NSString stringWithFormat:@"Can not find error %@", [NSString stringWithWstring:name]]];
        return 0;
    }
    
    int ThereAreNoFiles() {
        [delegat error:@"There are not files"];
        return 0;
    }
    
    int MessageError(const wchar_t *message) {
        [delegat error:[NSString stringWithFormat:@"Error: %@", [NSString stringWithWstring:message]]];
        return 0;
    }
};


@implementation Archive

-(instancetype)initWithName:(NSString *)name {
    
    if (self = [super init]) {
        _name = name;
    }
    
    return self;
}

-(NSArray *)list {
    
    std::vector<DirectoryItem> ra;
    CallbackTest cb(self.delegate);
    int ret = MLListArchive([self.name wstring], ra, cb);
    if (ret != 0) {
        [self.delegate error:[NSString stringWithWstring:GetErrorMessage(ret)]];
        return nil;
    }
    
    NSMutableArray *retArr = [[NSMutableArray alloc] init];
    for (std::vector<DirectoryItem>::iterator it = ra.begin(); it < ra.end(); it++)
    {
        DirectoryItem di = *it;
        
        Item *item = [[Item alloc] initWithName:[NSString stringWithWstring:di.name]
                                           size:di.size
                                 sizeCompressed:di.sizeCompressed
                                           date:@(di.date.c_str())
                                          attrs:@(di.attrs.c_str())];
        [retArr addObject:item];
    }
    
    return retArr;
    
}

-(int)compressItem:(NSArray *)items {
    return [self compressItem:items level:9];
}

-(int)compressItem:(NSArray *)items level:(NSInteger)compressionLevel {
    std::vector<std::wstring> itemsW;
    for (NSString *item in items) {
        itemsW.push_back([item wstring]);
    }
    CallbackTest cb(self.delegate);
    int ret = MLAddToArchive([self.name wstring], itemsW, cb, false, (int)compressionLevel, [self.workDir wstring]);
    if (ret) {
        [self.delegate error:[NSString stringWithWstring:GetErrorMessage(ret)]];
    }
    return ret;
}

-(int)decompressToDir:(NSString *)dir {
    return [self decompressItems:nil toDir:dir];
}

-(int)decompressItems:(NSArray *)items toDir:(NSString *)dir {
    std::vector<std::wstring> files;
    for (NSString *item : items) {
        files.push_back(item.wstring);
    }
    CallbackTest cb(self.delegate);
    int ret = MLExtractFromArchive([self.name wstring], [dir wstring], files, cb, [self.workDir wstring]);
    if (ret) {
        [self.delegate error:[NSString stringWithWstring:GetErrorMessage(ret)]];
    }
    return ret;
}

-(int)deleteItems:(NSArray *)items {
    std::vector<std::wstring> itemsW;
    for (NSString *item in items) {
        itemsW.push_back([item wstring]);
    }
    CallbackTest cb(self.delegate);
    int ret = MLDeleteFromArchive([self.name wstring], itemsW, cb, [self.workDir wstring]);
    if (ret) {
        [self.delegate error:[NSString stringWithWstring:GetErrorMessage(ret)]];
    }
    return ret;
}

-(NSString *)errorForCode:(NSInteger)code kind:(NSInteger)kind {
    return [NSString stringWithWstring:GetErrorMessage((int)code)];
}

@end
