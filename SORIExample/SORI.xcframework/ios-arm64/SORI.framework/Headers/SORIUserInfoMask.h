//
//  SORIUserInfoMask.h
//  SORI
//
//  Created by Heeseung Seo on 2017. 8. 28..
//  Copyright © 2017년 iPlateia. All rights reserved.
//

#import <Foundation/Foundation.h>

/*!
 @typedef SORIUserInfoMask
 A bit mask of user data fields, required for promotion application. With an exception of SORIUserInfoMaskNone every constant can be ORed to each other.
 @constant SORIUserInfoMaskNone
 No user data.
 @constant SORIUserInfoMaskName
 Mask value for the name.
 @constant SORIUserInfoMaskEmail
 Mask value for the e-mail address.
 @constant SORIUserInfoMaskBirthday
 Mask value for the birthday.
 @constant SORIUserInfoMaskMobile
 Mask value for the mobile number.
 @constant SORIUserInfoMaskAddress
 Mask value for the mailing address.
 @constant SORIUserInfoMaskGender
 Mask value for the gender.
 */
typedef NS_ENUM(NSInteger, SORIUserInfoMask) {
    SORIUserInfoMaskNone =      0,
    SORIUserInfoMaskName =      1,
    SORIUserInfoMaskEmail =     1 << 1,
    SORIUserInfoMaskBirthday =  1 << 2,
    SORIUserInfoMaskMobile =    1 << 3,
    SORIUserInfoMaskAddress =   1 << 4,
    SORIUserInfoMaskGender =    1 << 5,
};

/*!
 @function SORIUserInfoMaskFromRequiredArray:
     Generate SORIUserInfoMask values from promotion object.
 @param requiredArray
     The NSArray object contains string elements.
 */

SORIUserInfoMask SORIUserInfoMaskFromRequiredArray(NSArray * _Nonnull requiredArray);
