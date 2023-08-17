//
//  SORINotification.h
//  SORI
//
//  Created by Heeseung Seo on 2021/04/29.
//  Copyright © 2021 iPlateia. All rights reserved.
//

#ifndef SORINotification_h
#define SORINotification_h

#import <Foundation/Foundation.h>

/// This class defines notification information used inside the framework or for communication with the UI.
@interface SORINotification : NSObject

+ (NSNotificationName _Nonnull)audiopackLoadedNotificationName;

// - MARK: Helpers

+ (void)postAudiopackLoadedNotificationWithAFPCount:(NSInteger)afpCount;

@end

#endif /* SORINotification_h */
