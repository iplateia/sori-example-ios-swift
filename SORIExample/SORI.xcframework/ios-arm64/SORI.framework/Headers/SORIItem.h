//
//  SORIItem.h
//  SORIStatic
//
//  Created by Heeseung Seo on 2016. 12. 5..
//  Copyright © 2016년 iPlateia. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NSDictionary<NSString *, id> SORIItemDictionary;

/// This is just helper class.
/// You don't have to use it because you can access JSON data as it is.
@interface SORIItem : NSObject

@property (readonly, nonnull) SORIItemDictionary *dictionary;
@property (readonly, nonnull) NSArray<SORIItem *> *subitems;

#pragma mark - Main Properties

@property (readonly, nullable) NSString *identifier;
@property (readonly, nullable) NSString *title;
@property (readonly, nullable) NSString *name;
@property (readonly, nullable) NSString *subname;
@property (readonly, nullable) NSURL *imageURL;
@property (readonly, nullable) NSString *rel;
@property (readonly, nullable) NSDate *startDate;
@property (readonly, nullable) NSDate *endDate;
@property (readonly, nullable) NSString *type;

#pragma mark - Promotion Based Properties

@property (readonly) BOOL isMainPromotionItem;
@property (readonly, nullable) SORIItem *mainPromotionItem;

#pragma mark - Methods

- (instancetype _Nonnull)initWithDictionary:(SORIItemDictionary * _Nonnull)dictionary;

- (id _Nullable)objectForKey:(NSString * _Nonnull)key;
- (NSString * _Nullable)stringForKey:(NSString * _Nonnull)key;
- (SORIItemDictionary * _Nullable)dictionaryForKey:(NSString * _Nonnull)key;
- (NSURL * _Nullable)urlForKey:(NSString * _Nonnull)key;
- (NSDate * _Nullable)dateForKey:(NSString * _Nonnull)key;

@end
