/*!
 @author Heeseung Seo <hirenn@iplateia.com>
 @charset utf-8
 @copyright iPlateia Inc.
 */

#import <Foundation/Foundation.h>
#import <SORI/SORIChannel.h>
#import <SORI/SORIAudioMarker.h>

/// SORI Manager Detection Type
typedef NS_ENUM(int, SORIManagerDetectType) {
    /// This is not used
    SORIManagerDetectTypeInitial = 0,
    /// Legacy cloud recognition path.
    SORIManagerDetectTypeCloud
        DEPRECATED_MSG_ATTRIBUTE("Cloud recognition is legacy. Use the default server-managed recognition path."),
    /// Default server-managed recognition path.
    SORIManagerDetectTypeLocal,
    /// This is not used for 3rd party
    SORIManagerDetectTypeReserved
        DEPRECATED_MSG_ATTRIBUTE("Live recognition is legacy and not supported for third-party use.")
};

/// Legacy SORI API Version
typedef NS_ENUM(int, SORIAPIVersion) {
    /// SORI API Version 1
    SORIAPIVersionV1 = 1,
    /// SORI API Version 2
    SORIAPIVersionV2,
    /// SORI API Version 3
    SORIAPIVersionV3
};

/// Legacy response payload compatibility selector.
typedef NS_ENUM(int, SORIManagerResultType) {
    /// Legacy response model structure.
    SORIManagerResultTypeCloudCompatible
        DEPRECATED_MSG_ATTRIBUTE("Campaign response payloads are the default. Avoid configuring resultType in new code."),
    /// Simplified legacy response model structure.
    SORIManagerResultTypeLocal
        DEPRECATED_MSG_ATTRIBUTE("Campaign response payloads are the default. Avoid configuring resultType in new code."),
    /// SORI will result with the campaign response returned by /activity/.
    SORIManagerResultTypeCampaignResponse
};

typedef void (^SORIManagerCloudResponseHandler)(NSData *_Nullable data,
                                                NSError *_Nullable error);
typedef void (^SORIManagerRecognitionHandler)(NSDictionary *_Nullable media,
                                              NSError *_Nullable error);

/// Recognition ownership, independent of the authentication/sessionID property.
typedef NS_ENUM(NSInteger, SORIRecognitionLifecycleState) {
    SORIRecognitionLifecycleStateStarting = 0,
    SORIRecognitionLifecycleStateRunning = 1,
    SORIRecognitionLifecycleStateRecovering = 2,
    SORIRecognitionLifecycleStateStopped = 3
};
typedef void (^SORIRecognitionLifecycleHandler)(NSString *_Nonnull recognitionSessionID,
                                               SORIRecognitionLifecycleState state,
                                               NSError *_Nullable error);
/// Main-queue notification: recognitionSessionID (NSString), state (starting,
/// running, recovering, stopped), and error (NSError, only for stopped failure).
/// Starting is synchronous during start acceptance. Stopped follows cleanup.
FOUNDATION_EXPORT NSNotificationName _Nonnull const SORIRecognitionLifecycleNotification;
typedef void (^SORIManagerDetectMediaHandler)(NSDictionary *_Nullable media)
    __deprecated;
typedef void (^SORIManagerAudioPackLoadCompletionHandler)(BOOL succeed);
/// Copied raw PCM arrives on main; the pointer remains valid during the callback.
typedef void (^SORIManagerRawBufferHandler)(void *_Nonnull buffer, size_t size);
typedef void (^SORIManagerClearIDBCompletionHandler)(BOOL succeed);
typedef void (^SORIManagerMatchIntervalHandler)(NSTimeInterval interval);
typedef void (^SORIManagerAudioMarkerHandler)(SORIAudioMarker *_Nonnull audioMarker);
/// Returns caller metadata (string keys and values) for server recognition. See `metadataProvider`.
typedef NSDictionary<NSString *, NSString *> *_Nullable (^SORIRecognitionMetadataProvider)(void);
typedef void (^SORIManagerAudiomarkerChangeHandler)(NSString *_Nullable marker)
    DEPRECATED_MSG_ATTRIBUTE("Use SORIManagerAudioMarkerHandler. The legacy callback now receives only the Console-resolved marker name.");

/// How recognition uses SORI Console. Select it with `-[SORIManager setRecognitionMode:serverRecognition:error:]`
/// or `SORIAudioRecognizerConfiguration.recognitionMode`.
typedef NS_ENUM(NSInteger, SORIRecognitionMode) {
    /// Default. Recognition runs on the device only, as in 5.1.2: no server query, no channel state.
    /// Server recognition options must be nil.
    SORIRecognitionModeOnDevice = 0,
    /// Server recognition: no on-device material matcher; every query goes to the joint Console v1
    /// recognition/recording operation (`POST /api/v1/recognize`). Server recognition options are optional
    /// (nil means the defaults: continuous queries, no burst, 750 ms delay).
    SORIRecognitionModeServer = 1,
    /// On-device recognition matches every capture window at its normal cadence; in addition one window per
    /// `intervalMillis` (or every window during each burst) is also sent to the server. Server recognition
    /// options with a positive `intervalMillis` are required.
    SORIRecognitionModeOnDeviceWithPeriodicServer = 2,
};

/// Server recognition tuning: query cadence and delay. An immutable value object; equal values compare equal.
///
/// Every server query is one 52-hop (3.25 s) `afp-v1` window anchored on the capture sample clock.
/// Validation, applied together with the recognition mode (the same rules on every SORI SDK):
/// - `intervalMillis`: 0 sends every capture window (continuous, server mode only); a positive value sends one
///   window per period and must be at least 3250 (and at most 9223372036854, so no time arithmetic
///   overflows). On-device recognition with periodic server recognition
///   requires a positive value.
/// - `burstMillis`: 0 sends one window per period; otherwise every window is sent during the first `burstMillis`
///   of each period, with 3250 <= `burstMillis` < `intervalMillis`. The microphone stays on between bursts.
/// - `delayMillis`: 0 to 60000. The queried window ends `delayMillis` before the newest captured audio
///   (default 750; the extractor's 625 ms lookahead counts towards it).
@interface SORIServerRecognitionOptions : NSObject <NSCopying>
/// Server query period in milliseconds; 0 is continuous. Default 0.
@property(nonatomic, readonly) NSInteger intervalMillis;
/// Burst at the start of each period in milliseconds; 0 is one window per period. Default 0.
@property(nonatomic, readonly) NSInteger burstMillis;
/// Age of the queried window's end relative to the newest captured audio, in milliseconds. Default 750.
@property(nonatomic, readonly) NSInteger delayMillis;
/// Defaults: interval 0, burst 0, delay 750.
- (nonnull instancetype)init;
/// Burst 0 and delay 750.
- (nonnull instancetype)initWithIntervalMillis:(NSInteger)intervalMillis NS_SWIFT_NAME(init(intervalMillis:));
/// Delay 750.
- (nonnull instancetype)initWithIntervalMillis:(NSInteger)intervalMillis burstMillis:(NSInteger)burstMillis
    NS_SWIFT_NAME(init(intervalMillis:burstMillis:));
/// Stores the values as given; they are validated with the recognition mode when the configuration is applied.
- (nonnull instancetype)initWithIntervalMillis:(NSInteger)intervalMillis burstMillis:(NSInteger)burstMillis
                                   delayMillis:(NSInteger)delayMillis
    NS_DESIGNATED_INITIALIZER NS_SWIFT_NAME(init(intervalMillis:burstMillis:delayMillis:));
@end

@class SORIApplicationRequest;
@class SORIContinuousHitManager;

/// The main class for SORI API. There should be at most one instance throughout the entire application;
/// multiple instances can cause an unexpected behavior due to the limitation on the usage of device microphone.
@interface SORIManager : NSObject

/// SORI Application ID
@property(nonatomic, strong, nullable) NSString *applicationID;

/// SORI Secret Key
@property(nonatomic, strong, nullable) NSString *secretKey;

/// SORI API Endpoint
@property (nonatomic, strong, nonnull) NSString *apiEndpoint;

/// Legacy SORI API Version
@property(nonatomic, assign) SORIAPIVersion apiVersion;

/// Legacy response payload compatibility selector. Defaults to SORIManagerResultTypeCampaignResponse.
@property(nonatomic, assign) SORIManagerResultType resultType
    DEPRECATED_MSG_ATTRIBUTE("Campaign response payloads are the default. Avoid configuring resultType in new code.");

/// If YES, SORI prepared successfully.
@property(nonatomic, readonly) BOOL prepared;

/// If YES, the SORI running.
@property(nonatomic, readonly) BOOL running;

/// Current pending/active recognition ID; nil after stop or terminal failure.
@property(atomic, copy, readonly, nullable) NSString *recognitionSessionID;

/// If YES, the Audio Recorder is running.
/// This is independent and get value from recorder service directly,
/// So may the value is different with value of property running.
@property(nonatomic, readonly) BOOL isRecorderRunning;

/// This property is deprecated. Use hitManager property instead.
/// If this value was YES, SORI will detect duplicated media which is detect previous.
/// Default value is YES.
@property(nonatomic, assign) BOOL detectFreshResultOnly
    DEPRECATED_MSG_ATTRIBUTE("use hitManager instead.");

/// SORI API Cutoff value
@property(nonatomic, assign) double cutoff;

/// If YES, SORI will filter detected medias by cutoff score.
/// Default value is YES.
@property(nonatomic, assign) BOOL useCutoffFilter;

/// If YES, SORI will send user's current location with server requests.
@property(nonatomic, assign) BOOL useLocationService;
/// Caller metadata for server recognition queries (either server recognition mode), for example your own user or session
/// identifiers. SORI Console returns the map as `metadata` in `live.recognition` and `live.segment`
/// webhooks. Called on the main thread; return quickly. nil or an empty map sends nothing.
/// It is evaluated once per `/v1/recognize` query, when the query window is frozen (captured), and that snapshot
/// is sent unchanged however long the query waits for the delay hold or a busy transport, so a webhook sees the
/// state in effect when the audio was captured. Setting a new provider applies to the next frozen window without
/// a restart. Before sending, the SDK drops
/// non-string, blank and reserved keys (`channel`, `channel_id`, `channel_material_id`,
/// `channel_mapping_revision`, `channel_resolution`, `live_score`, `live_position_ms`, `program`) and keys
/// longer than 64 Unicode scalars, truncates values to 512 Unicode scalars and keeps the first 32 keys in
/// sorted order. The legacy `/activity/` request never carries it.
@property(atomic, copy, nullable) SORIRecognitionMetadataProvider metadataProvider;

/// If YES, local recognition also runs near-ultrasonic audiomarker detection.
/// Default value is NO.
@property(nonatomic, assign) BOOL audiomarker;
/// The configured recognition mode. Default SORIRecognitionModeOnDevice. Set it with
/// `setRecognitionMode:serverRecognition:error:`.
/// While recognition runs this is the configuration the next `startWithRepeat:...` call applies, which may differ
/// from the one the running session uses.
@property(nonatomic, readonly) SORIRecognitionMode recognitionMode;
/// The configured server recognition options; nil in on-device mode, and nil (the defaults) in server mode when
/// none were given. Set them with `setRecognitionMode:serverRecognition:error:`.
@property(nonatomic, copy, readonly, nullable) SORIServerRecognitionOptions *serverRecognition;
@property(nonatomic, copy, nullable) SORIChannelStateHandler channelStateHandler;
@property(nonatomic, readonly, nonnull) SORIChannelSnapshot *currentChannelState;
/// Called once per continuous activity segment after SORI Console resolves a marker.
/// Local codebook labels are never delivered. No callback occurs unless the
/// activity response contains both a non-empty marker identifier and name.
@property(nonatomic, copy, nullable) SORIManagerAudioMarkerHandler audioMarkerHandler;

/// Current SDK-owned state; reads synchronously enforce expiry.
@property(nonatomic, readonly, nonnull) SORIAudioMarkerSnapshot *currentAudioMarkerState;
/// Main-queue delivery, including the current snapshot on attachment. Set nil to detach.
@property(nonatomic, copy, nullable) SORIAudioMarkerStateHandler audioMarkerStateHandler;
/// Exact server activity results, including independent late marker upgrades.
@property(nonatomic, copy, nullable) SORIActivityResultHandler activityResultHandler;

/// Legacy string callback retained for source and binary compatibility.
/// When set, it receives the Console-resolved marker name at the same time as
/// `audioMarkerHandler`. It never receives a local codebook label or nil clear.
@property(nonatomic, copy, nullable) SORIManagerAudiomarkerChangeHandler audiomarkerChangeHandler
    DEPRECATED_MSG_ATTRIBUTE("Use audioMarkerHandler for the Console-resolved identifier and name.");

/// Compatibility policy for reserved/live detection only. Local fingerprint and
/// marker recognition use ordered activity runs, independent of this helper.
@property(nonatomic, strong, nullable) SORIContinuousHitManager *hitManager;

/// If YES, logging SORI internal logs.
/// PS. This will put very huge messages to your log console.
@property(nonatomic, assign) BOOL debugMode;

/// This property was deprecated.
/// If YES, SORIManager will work by old way (V1)
/// You cannot use legacy mode with the default recognition path.
@property(nonatomic, assign)
    BOOL useLegacy DEPRECATED_MSG_ATTRIBUTE("use apiVersion instead.");

/// If YES, SORI will show network indicator on iOS status bar when using network connection.
/// Default value is YES.
@property(nonatomic, assign) BOOL showNetworkActivityIndicator;

/// Assign offline debuf file path.
/// Default value is nil.
@property(nonatomic, strong, nullable) NSString *debugLogFilePath;

/// You can take raw response data from server from this handler.
@property(nonatomic, strong, nullable) SORIManagerCloudResponseHandler cloudResponseHandler;

/// This property shows current session ID.
@property(nonatomic, readonly, nonnull) NSString *sessionID;

/// Raw Audio Buffer will provide when you assign this handler. (EXPERIMENTAL)
@property(nonatomic, strong, nullable) SORIManagerRawBufferHandler rawBufferHandler;

/// Just use to get match interval of each recognition request. (EXPERIMENTAL)
@property(nonatomic, strong, nullable) SORIManagerMatchIntervalHandler matchIntervalHandler;

/// Optional explicit model/audio-pack path for compatibility.
/// Normal integrations should leave this unset and use the framework-bundled model.
@property(nonatomic, strong, nullable) NSString *baseModelPath;

/// SORI will updates audiopacks after this time, unless nil.
/// NOTE: Operation time is not guaranteed because it depends on timer interval.
@property(nonatomic, strong, nullable) NSDate *nextUpdateDate;

/// SORIManager singleton factory
+ (SORIManager *_Nonnull)sharedManager;

/// Not documented
- (NSURL *_Nonnull)apiURLWithSuffix:(NSString *_Nonnull)suffix;

/// Prepare SORIManager for server-managed recognition.
/// This contains authentication, download audio pack, and related processes.
/// You usually do not need to call this directly because startWithRepeat:recognitionHandler: prepares automatically.
/// You cannot call this when useLegacy is YES(true).
/// Do not use this with prepareWithCustomAudioPackPaths method together.
- (void)prepare;

/// Prepare SORIManager with an explicit legacy detection type.
/// This contains authentication, download audio pack, and related processes.
/// You cannot call this when useLegacy is YES(true).
/// Do not use this with prepareWithCustomAudioPackPaths method together.
/// @param type
///     SORIManagerDetectType value
- (void)prepareWithType:(SORIManagerDetectType)type;

/// Prepare SORIManager with explicit audio pack paths.
/// This is a compatibility path and is not recommended for normal integrations.
/// You cannot call when useLegacy is YES(true).
/// Do not use this with prepare method together.
/// @param paths
///     Array of audio pack file paths
- (void)prepareWithCustomAudioPackPaths:(NSArray<NSString *> *_Nonnull)paths;

/// Deprecated
- (void)checkRecentAudioPack:
    (void (^_Nullable)(NSURL *_Nullable audioPackURL))updateAvailable
    __deprecated;

/// Load audio pack files to memory.
/// Calling this method is not recommended.
/// @param paths
///     Array of audio pack file paths
/// @param completionHandler
///     Calling when finished to load
- (void)loadAudioPackPaths:(NSArray<NSString *> *_Nonnull)paths
         completionHandler:(SORIManagerAudioPackLoadCompletionHandler _Nullable)
                               completionHandler;

/// Check recent audio pack and download if available new version.
/// This method will calling automatically.
/// But if you want to check update manually, call this method.
- (void)checkUpdateAudioPacks;

/// Remove all loaded audio packs
- (void)removeAllAudioPacks;

// NOTE: Legacy Support

/// Start SORI Manager. (This is legacy support. use startWithRepeat:recognitionHandler instead).
/// @param type
///     Detection Type
/// @param repeat
///     If YES, SORI not stop when detect any media.
/// @param recognitionHandler
///     Calling if detect any media
- (void)startWithType:(SORIManagerDetectType)type
                repeat:(BOOL)repeat
    recognitionHandler:
        (SORIManagerRecognitionHandler _Nullable)recognitionHandler;

/// Start SORI Manager. This prepares server-managed recognition automatically when needed.
/// @param repeat
///     If YES, SORI not stop when detect any media.
/// @param recognitionHandler
///     Calling if detect any media
- (void)startWithRepeat:(BOOL)repeat
     recognitionHandler:
         (SORIManagerRecognitionHandler _Nullable)recognitionHandler;

/// Claims and returns recognition ownership synchronously on the main queue.
/// Repeated pending/active starts retain the first arguments/handlers and ID; such a start is also the point where
/// a server recognition tuning change is applied to the session (see `setRecognitionMode:serverRecognition:error:`).
/// Lifecycle callbacks are queued on main and retain the originating ID/handler.
- (NSString *_Nonnull)startWithRepeat:(BOOL)repeat
                  recognitionHandler:(SORIManagerRecognitionHandler _Nullable)recognitionHandler
                    lifecycleHandler:(SORIRecognitionLifecycleHandler _Nullable)lifecycleHandler;

/// Stop SORI Manager
- (void)stop;

/// Configures the recognition mode and its server recognition options. The three minimal configurations:
///
///     [manager setRecognitionMode:SORIRecognitionModeOnDevice serverRecognition:nil error:&error];   // default
///     [manager setRecognitionMode:SORIRecognitionModeServer serverRecognition:nil error:&error];     // continuous
///     [manager setRecognitionMode:SORIRecognitionModeOnDeviceWithPeriodicServer
///               serverRecognition:[[SORIServerRecognitionOptions alloc] initWithIntervalMillis:300000]
///                           error:&error];
///
/// Returns NO with SORIErrorCodeInvalidConfiguration, changing nothing, when the pair is invalid: options with the
/// on-device mode, on-device with periodic server recognition without a positive interval, a positive interval
/// below 3250, a burst outside `3250 <= burst < interval`, a delay outside 0...60000, or either server mode while
/// `audiomarker` is YES. See `SORIServerRecognitionOptions`.
///
/// The configuration is applied by the next `startWithRepeat:...` call, never immediately: a running session keeps
/// the configuration it was started with and is never stopped by this call. When a session is already running (or
/// starting), the next `startWithRepeat:...` call keeps returning the same recognition ID and:
/// - with only a different interval, burst or delay, applies them to the running session in place, from the next
///   captured window, without restarting the recorder, capture, the audio session or the on-device matcher; the
///   server query already in flight is kept;
/// - with a different recognition mode, changes nothing; the new configuration is used by the first start after
///   the session stops;
/// - with no change, does nothing.
/// Callable from any thread; applied on the main thread.
- (BOOL)setRecognitionMode:(SORIRecognitionMode)recognitionMode
         serverRecognition:(nullable SORIServerRecognitionOptions *)serverRecognition
                     error:(NSError *_Nullable *_Nullable)error
    NS_SWIFT_NAME(setRecognitionMode(_:serverRecognition:));

/// Clear last recognition media ID.
/// This allows SORI to detect previous detected media again.
- (void)clearState;

/// Remove all informations and files for SORI.
/// This is not available when SORIManager is running.
/// Please do not call this method if you have any no problem to use SORI.
- (void)reset;

/// Remove the recognizer database. (It's not recommend to call this without any knowledge about this.)
- (void)clearIDBWithCompletionHandler:
    (SORIManagerClearIDBCompletionHandler _Nullable)completionHandler;

/// logging recognizer information for detailed logs
- (void)logLocalInformations;

#pragma mark - Permissions

/// This method returns whether the audio device permission has been granted.
- (bool)audioRecordingPermitted;

@end
