//
//  SORIContinuousHitManager.h
//  SORI
//
//  Created by Heeseung Seo on 2020/03/04.
//  Copyright © 2020 iPlateia. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(int, SORIContinuousHitPolicy) {
    SORIContinuousHitPolicyNone = 0,
    SORIContinuousHitPolicyRestrict,
    SORIContinuousHitPolicyStaticTimeout,
    SORIContinuousHitPolicyDynamicTimeout
};

NS_ASSUME_NONNULL_BEGIN

/// In-memory compatibility policy for reserved/live detection and direct callers.
/// Local material recognition uses SORILogStore's bounded ordered runs instead.
@interface SORIContinuousHitManager : NSObject

@property (nonatomic, assign) SORIContinuousHitPolicy policy;
@property (nonatomic, assign) NSTimeInterval timeout;
@property (nonatomic, assign) NSInteger multipier;

- (BOOL)canHitWithIdentifier:(NSString *)identifier length:(NSTimeInterval)length;
- (BOOL)canHitWithMediaDictionary:(NSDictionary * _Nonnull)media;

- (void)hitWithIdentifier:(NSString *)identifier length:(NSTimeInterval)length;
- (void)hitWithMediaDictionary:(NSDictionary * _Nonnull)media;

- (void)reset;

@end

NS_ASSUME_NONNULL_END
