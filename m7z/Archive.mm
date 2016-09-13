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

class M7ZArchiveCallback: public MLUpdateCallback, public MLExtractCallback
{
    
public:
    id<M7ZArchiveDelegate> delegat;
    
    M7ZArchiveCallback(id<M7ZArchiveDelegate> delegat): delegat(delegat) {}
    
    int SetTotal(unsigned long long size) {
        delegat.total = size;
        return 0;
    }
    
    int CompressingItem(const wchar_t *name, bool isAnti, int mode){
        [delegat item:[NSString stringWithWstring:name] mode:1000 + mode];
        return delegat.shouldBreak;
    }
    
    int DecompressingItem(const wchar_t *name, bool isFolder, int mode, const unsigned long long *position) {
        [delegat item:[NSString stringWithWstring:name] mode:2000 + mode];
        return 0;
    }
    
    int SetCompleted(const unsigned long long *completeValue) {
        delegat.completed = *completeValue;
        return delegat.shouldBreak;
    }
    
    int SetOperationResult(int x, int kind) {
        if (x == 0) return 0;
        
        BOOL ret = YES;
        switch (kind)
        {
            case 1:
                ret = [delegat error:[NSString stringWithWstring:GetExtractOperationErrorMessage(x)] resultCode:1000 + x];
                break;
                
            case 2:
                ret = [delegat error:[NSString stringWithWstring:GetUpdateOperationErrorMessage(x)] resultCode:2000 + x];
                break;
        }
        
        return (ret)? 0 : x;
    }
    
    int FinishArchive() {
        [delegat done];
        return 0;
    }
    
    int AskOverwrite (
        const wchar_t *existName, const time_t *existTime, const unsigned long long *existSize,
        const wchar_t *newName, const time_t *newTime, const unsigned long long *newSize,
        int *answer) {
        
        *answer = [delegat shouldOverwriteItem:(existName) ? [NSString stringWithWstring:existName] : @""
                                          date:(existTime) ? [NSDate dateWithTimeIntervalSince1970:*existTime] : nil
                                          size:(existSize) ? *existSize : 0
                                       newName:(newName) ? [NSString stringWithWstring:newName] : @""
                                       newDate:(newTime) ? [NSDate dateWithTimeIntervalSince1970:*newTime] : nil
                                       newSize:(newSize) ? *newSize : 0];
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

    int OpenFileError(const wchar_t *name, int systemError) {
        [delegat error:[NSString stringWithFormat:@"Open file error %@", [NSString stringWithWstring:name]]
            resultCode:M7Z_ORC_OtherError];
        return 0;
    }
    
    int CanNotFindError(const wchar_t *name, int systemError) {
        [delegat error:[NSString stringWithFormat:@"Can not find error %@", [NSString stringWithWstring:name]]
            resultCode:M7Z_ORC_OtherError];
        return 0;
    }
    
    int MessageError(const wchar_t *message) {
        [delegat error:[NSString stringWithFormat:@"Error: %@", [NSString stringWithWstring:message]]
            resultCode:M7Z_ORC_OtherError];
        return 0;
    }
};


@implementation M7ZArchive

-(instancetype)initWithName:(NSString *)name {
    
    if (self = [super init]) {
        _name = name;
    }
    
    return self;
}

-(int)listItemsTo:(NSMutableArray *)output {
    
    std::vector<DirectoryItem> ra;
    M7ZArchiveCallback cb(self.delegate);
    int ret = MLListArchive([self.name wstring], ra, cb);
    if (ret != 0) {
        [self.delegate error:[NSString stringWithWstring:GetErrorMessage(ret)]
                  resultCode:M7Z_ORC_OtherError];
        return ret;
    }
    
    for (std::vector<DirectoryItem>::iterator it = ra.begin(); it < ra.end(); it++)
    {
        DirectoryItem di = *it;
        
        M7ZItem *item = [[M7ZItem alloc] initWithName:[NSString stringWithWstring:di.name]
                                           size:di.size
                                 sizeCompressed:di.sizeCompressed
                                           date:@(di.date.c_str())
                                          attrs:@(di.attrs.c_str())];
        [output addObject:item];
    }
    
    return 0;
    
}

-(int)addItems:(NSArray *)items {
    return [self addItems:items encryptHeader:NO compressionLevel:5];
}

-(int)addItems:(NSArray *)items encryptHeader:(BOOL)encryptHeader {
    return [self addItems:items encryptHeader:encryptHeader compressionLevel:5];
}

-(int)addItems:(NSArray *)items encryptHeader:(BOOL)encryptHeader compressionLevel:(NSInteger)compressionLevel {
    std::vector<std::wstring> itemsW;
    for (NSString *item in items) {
        itemsW.push_back([item wstring]);
    }
    M7ZArchiveCallback cb(self.delegate);
    int ret = MLAddToArchive([self.name wstring], itemsW, cb, false, (int)compressionLevel, [self.workDir wstring]);
    if (ret) {
        [self.delegate error:[NSString stringWithWstring:GetErrorMessage(ret)]
                  resultCode:M7Z_ORC_OtherError];
    }
    return ret;
}

-(int)extractAllToDir:(NSString *)dir {
    return [self extractItems:nil toDir:dir];
}

-(int)extractItems:(NSArray *)items toDir:(NSString *)dir {
    std::vector<std::wstring> files;
    for (NSString *item : items) {
        files.push_back(item.wstring);
    }
    M7ZArchiveCallback cb(self.delegate);
    int ret = MLExtractFromArchive([self.name wstring], [dir wstring], files, cb, [self.workDir wstring]);
    if (ret) {
        [self.delegate error:[NSString stringWithWstring:GetErrorMessage(ret)]
                  resultCode:M7Z_ORC_OtherError];
    }
    return ret;
}

-(int)deleteItems:(NSArray *)items {
    std::vector<std::wstring> itemsW;
    for (NSString *item in items) {
        itemsW.push_back([item wstring]);
    }
    M7ZArchiveCallback cb(self.delegate);
    int ret = MLDeleteFromArchive([self.name wstring], itemsW, cb, [self.workDir wstring]);
    if (ret) {
        [self.delegate error:[NSString stringWithWstring:GetErrorMessage(ret)]
                  resultCode:M7Z_ORC_OtherError];
    }
    return ret;
}

-(NSString *)errorForCode:(NSInteger)code {
    return [NSString stringWithWstring:GetErrorMessage((int)code)];
}

@end
