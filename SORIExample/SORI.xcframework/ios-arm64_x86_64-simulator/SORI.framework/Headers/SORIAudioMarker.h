//
//  SORIAudioMarker.h
//  SORI
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// Immutable SORI Console-managed audio-marker identity.
///
/// Instances are created only after an activity response contains both a
/// non-empty `audio_marker_id` and `audio_marker_name`. Local codebook labels
/// are recognition implementation details and are never represented here.
@interface SORIAudioMarker : NSObject <NSCopying>

/// SORI Console audio-marker id returned as `audio_marker_id`.
@property(nonatomic, copy, readonly) NSString *identifier;

/// SORI Console audio-marker name returned as `audio_marker_name`.
@property(nonatomic, copy, readonly) NSString *name;

- (instancetype)initWithIdentifier:(NSString *)identifier
                               name:(NSString *)name NS_DESIGNATED_INITIALIZER
    NS_SWIFT_NAME(init(identifier:name:));

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END

NS_ASSUME_NONNULL_BEGIN

/// SDK-owned current capture state. Only Identified carries a Console identity.
typedef NS_ENUM(NSInteger, SORIAudioMarkerState) {
    SORIAudioMarkerStateInactive,
    SORIAudioMarkerStateListening,
    SORIAudioMarkerStateDetected,
    SORIAudioMarkerStateIdentified,
    SORIAudioMarkerStateCleared,
};

/// Immutable snapshot. Reading or observing it never renews microphone evidence.
@interface SORIAudioMarkerSnapshot : NSObject <NSCopying>
@property(nonatomic, readonly) SORIAudioMarkerState state;
@property(nonatomic, readonly) NSUInteger captureGeneration;
@property(nonatomic, readonly) NSUInteger revision;
@property(nonatomic, readonly, nullable) SORIAudioMarker *audioMarker;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
@end

/// A server-accepted activity, delivered again with the same id after enrichment.
/// Keep completed results when the current marker is cleared.
@interface SORIActivityResult : NSObject <NSCopying>
@property(nonatomic, copy, readonly) NSString *activityID;
@property(nonatomic, copy, readonly) NSString *materialID;
@property(nonatomic, readonly) NSUInteger captureGeneration;
@property(nonatomic, readonly, getter=isRefinement) BOOL refinement;
@property(nonatomic, readonly, nullable) SORIAudioMarker *audioMarker;
/// Console response with private detector labels removed.
@property(nonatomic, copy, readonly) NSDictionary *response;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
@end

typedef void (^SORIAudioMarkerStateHandler)(SORIAudioMarkerSnapshot *snapshot);
typedef void (^SORIActivityResultHandler)(SORIActivityResult *result);

NS_ASSUME_NONNULL_END
