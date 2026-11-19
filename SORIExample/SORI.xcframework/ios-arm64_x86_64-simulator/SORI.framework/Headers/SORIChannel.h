#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, SORIChannelState) {
    SORIChannelStateInactive,
    SORIChannelStateListening,
    SORIChannelStateIdentified,
    SORIChannelStateUnresolved,
    SORIChannelStateCleared,
};

typedef NS_ENUM(NSInteger, SORIChannelBroadcastType) {
    SORIChannelBroadcastTypeUnknown,
    SORIChannelBroadcastTypeTV,
    SORIChannelBroadcastTypeRadio,
};

/// Authorized Console identity. A LIVE material identifier is never a channel ID.
@interface SORIChannelIdentity : NSObject <NSCopying>
@property(nonatomic, copy, readonly) NSString *identifier;
@property(nonatomic, copy, readonly) NSString *name;
@property(nonatomic, readonly) SORIChannelBroadcastType broadcastType;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
@end

/// Optional episode object supplied by Console; each member is independently nullable.
@interface SORIChannelEpisode : NSObject <NSCopying>
@property(nonatomic, copy, readonly, nullable) NSNumber *number;
@property(nonatomic, copy, readonly, nullable) NSNumber *part;
@property(nonatomic, copy, readonly, nullable) NSString *title;
/// Immutable JSON object, including unknown fields from compatible servers.
@property(nonatomic, copy, readonly) NSDictionary *response;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
@end

/// Optional collected EPG context supplied by the server with this observation.
/// Unknown fields are preserved in response. This is not acoustic program recognition.
@interface SORIChannelProgram : NSObject <NSCopying>
/// Optional future identity; current Console responses do not require or synthesize it.
@property(nonatomic, copy, readonly, nullable) NSString *identifier;
/// Original source title, distinct from the normalized name supplied by Console.
@property(nonatomic, copy, readonly, nullable) NSString *title;
@property(nonatomic, copy, readonly, nullable) NSString *name;
@property(nonatomic, readonly, nullable) SORIChannelEpisode *episode;
/// Parsed public start_at/end_at instants. Missing or malformed fields remain nil.
@property(nonatomic, copy, readonly, nullable) NSDate *startAt;
@property(nonatomic, copy, readonly, nullable) NSDate *endAt;
/// Compatibility aliases for native consumers; these are not plural wire keys.
@property(nonatomic, copy, readonly, nullable) NSDate *startsAt;
@property(nonatomic, copy, readonly, nullable) NSDate *endsAt;
/// Optional future reference metadata; not invented from current v1 EPG data.
@property(nonatomic, copy, readonly, nullable) NSDate *referenceObservedAt;
@property(nonatomic, copy, readonly) NSDictionary *response;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
@end

/// Last observed channel evidence, independent of advertisement/campaign results.
/// UTC dates and monotonic seconds use distinct clocks. Reading/replaying never
/// extends expiry and does not prove uninterrupted viewing between queries.
@interface SORIChannelSnapshot : NSObject <NSCopying>
@property(nonatomic, readonly) SORIChannelState state;
@property(nonatomic, readonly, nullable) SORIChannelIdentity *channel;
@property(nonatomic, readonly, nullable) SORIChannelProgram *program;
@property(nonatomic, copy, readonly, nullable) NSString *observationID;
@property(nonatomic, copy, readonly, nullable) NSDate *observedAt;
@property(nonatomic, readonly) NSTimeInterval observedMonotonicTime;
@property(nonatomic, readonly) NSTimeInterval expiresMonotonicTime;
@property(nonatomic, copy, readonly, nullable) NSString *clearReason;
@property(nonatomic, readonly) NSUInteger captureGeneration;
@property(nonatomic, readonly) NSUInteger policyGeneration;
@property(nonatomic, readonly) NSUInteger revision;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
@end

typedef void (^SORIChannelStateHandler)(SORIChannelSnapshot *snapshot);

NS_ASSUME_NONNULL_END
