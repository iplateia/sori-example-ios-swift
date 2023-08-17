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

/// This class is used to define rules when recognizing same audio materials continuously.
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
