//
//  SORI.h
//  SORI
//
//  Created by Heeseung Seo on 2016. 12. 14..
//  Copyright © 2016년 iPlateia. All rights reserved.
//

#import <Foundation/Foundation.h>

//! Project version number for SORI.
extern double SORIVersionNumber;

//! Project version string for SORI.
extern const unsigned char SORIVersionString[];

//! Public SDK version string, including pre-release labels when applicable.
FOUNDATION_EXPORT NSString * const SORISDKVersion;

// In this header, you should import all the public headers of your framework using statements like #import <SORI/PublicHeader.h>


#import <SORI/SORIManager.h>
#import <SORI/SORIAudioRecognizer.h>
//#import <SORI/SORIApplicationRequest.h>
#import <SORI/SORIItem.h>
#import <SORI/SORIUserInfoMask.h>
#import <SORI/SORIError.h>
#import <SORI/SORINotification.h>
#import <SORI/SORIContinuousHitManager.h>
