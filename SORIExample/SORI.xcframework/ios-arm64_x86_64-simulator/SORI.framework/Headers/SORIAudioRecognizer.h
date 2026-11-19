//
//  SORIAudioRecognizer.h
//  SORI
//

#import <Foundation/Foundation.h>
#import <SORI/SORIManager.h>

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(SORIAudioRecognizer.Configuration)
@interface SORIAudioRecognizerConfiguration : NSObject <NSCopying>

@property(nonatomic, copy, nullable) NSString *applicationID;
@property(nonatomic, copy, nullable) NSString *secretKey;
@property(nonatomic, copy) NSString *apiEndpoint;
@property(nonatomic, assign) SORIAPIVersion apiVersion;
/// Legacy response payload compatibility selector. Defaults to SORIManagerResultTypeCampaignResponse.
@property(nonatomic, assign) SORIManagerResultType resultType
    DEPRECATED_MSG_ATTRIBUTE("Campaign response payloads are the default. Avoid configuring resultType in new code.");
@property(nonatomic, assign) double cutoff;
@property(nonatomic, assign) BOOL useCutoffFilter;
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
@property(nonatomic, copy, nullable) SORIRecognitionMetadataProvider metadataProvider;
/// If YES, local recognition also runs near-ultrasonic audiomarker detection.
/// Defaults to NO so recognition remains fingerprint-only unless explicitly enabled.
@property(nonatomic, assign) BOOL audiomarker;
/// Recognition mode. Default SORIRecognitionModeOnDevice. See `-[SORIManager setRecognitionMode:serverRecognition:error:]`
/// for the validation rules and for when a change reaches a running session.
@property(nonatomic, assign) SORIRecognitionMode recognitionMode;
/// Server recognition options: nil in on-device mode; optional in server mode (nil = defaults); required, with a
/// positive interval, in on-device mode with periodic server recognition. Default nil.
@property(nonatomic, copy, nullable) SORIServerRecognitionOptions *serverRecognition;
@property(nonatomic, copy, nullable) SORIChannelStateHandler channelStateHandler;
/// Called once per continuous activity segment after SORI Console resolves a marker.
@property(nonatomic, copy, nullable) SORIManagerAudioMarkerHandler audioMarkerHandler;
@property(nonatomic, copy, nullable) SORIAudioMarkerStateHandler audioMarkerStateHandler;
@property(nonatomic, copy, nullable) SORIActivityResultHandler activityResultHandler;
/// Legacy callback receiving only the Console-resolved marker name.
@property(nonatomic, copy, nullable) SORIManagerAudiomarkerChangeHandler audiomarkerChangeHandler
    DEPRECATED_MSG_ATTRIBUTE("Use audioMarkerHandler for the Console-resolved identifier and name.");
/// Reserved/live compatibility policy; local material runs do not consult it.
@property(nonatomic, strong, nullable) SORIContinuousHitManager *hitManager;
@property(nonatomic, assign) BOOL debugMode;
@property(nonatomic, assign) BOOL showNetworkActivityIndicator;
@property(nonatomic, copy, nullable) NSString *debugLogFilePath;
@property(nonatomic, copy, nullable) SORIManagerCloudResponseHandler cloudResponseHandler;
@property(nonatomic, copy, nullable) SORIManagerRawBufferHandler rawBufferHandler;
@property(nonatomic, copy, nullable) SORIManagerMatchIntervalHandler matchIntervalHandler;
@property(nonatomic, copy, nullable) NSString *baseModelPath;
@property(nonatomic, strong, nullable) NSDate *nextUpdateDate;

- (instancetype)init;
- (instancetype)initWithApplicationID:(NSString *)applicationID
                            secretKey:(NSString *)secretKey
    NS_SWIFT_NAME(init(applicationID:secretKey:));

@end

/// Public recognizer facade backed by the shared SORIManager instance.
@interface SORIAudioRecognizer : NSObject

@property(nonatomic, strong, nullable) NSString *applicationID;
@property(nonatomic, strong, nullable) NSString *secretKey;
@property(nonatomic, strong) NSString *apiEndpoint;
@property(nonatomic, assign) SORIAPIVersion apiVersion;
@property(nonatomic, assign) SORIManagerResultType resultType
    DEPRECATED_MSG_ATTRIBUTE("Campaign response payloads are the default. Avoid configuring resultType in new code.");
@property(nonatomic, readonly) BOOL prepared;
@property(nonatomic, readonly) BOOL running;
/// Current pending/active recognition ownership; nil after cleanup.
@property(atomic, copy, readonly, nullable) NSString *recognitionSessionID;
@property(nonatomic, readonly) BOOL isRecorderRunning;
@property(nonatomic, assign) double cutoff;
@property(nonatomic, assign) BOOL useCutoffFilter;
@property(nonatomic, assign) BOOL useLocationService;
/// Caller metadata for server recognition. See `SORIAudioRecognizerConfiguration.metadataProvider`.
@property(nonatomic, copy, nullable) SORIRecognitionMetadataProvider metadataProvider;
/// If YES, local recognition also runs near-ultrasonic audiomarker detection.
/// Defaults to NO so recognition remains fingerprint-only unless explicitly enabled.
@property(nonatomic, assign) BOOL audiomarker;
/// Configured recognition mode. See `SORIManager.recognitionMode`.
@property(nonatomic, readonly) SORIRecognitionMode recognitionMode;
/// Configured server recognition options. See `SORIManager.serverRecognition`.
@property(nonatomic, copy, readonly, nullable) SORIServerRecognitionOptions *serverRecognition;
@property(nonatomic, copy, nullable) SORIChannelStateHandler channelStateHandler;

/// Last observed channel snapshot; reading it does not renew evidence.
@property(nonatomic, readonly) SORIChannelSnapshot *currentChannelState;
/// Current SDK snapshot; never renews microphone evidence.
@property(nonatomic, readonly) SORIAudioMarkerSnapshot *currentAudioMarkerState;
/// Called once per continuous activity segment after SORI Console resolves a marker.
@property(nonatomic, copy, nullable) SORIManagerAudioMarkerHandler audioMarkerHandler;
@property(nonatomic, copy, nullable) SORIAudioMarkerStateHandler audioMarkerStateHandler;
@property(nonatomic, copy, nullable) SORIActivityResultHandler activityResultHandler;
/// Legacy callback receiving only the Console-resolved marker name.
@property(nonatomic, copy, nullable) SORIManagerAudiomarkerChangeHandler audiomarkerChangeHandler
    DEPRECATED_MSG_ATTRIBUTE("Use audioMarkerHandler for the Console-resolved identifier and name.");
/// Reserved/live compatibility policy; local material runs do not consult it.
@property(nonatomic, strong, nullable) SORIContinuousHitManager *hitManager;
@property(nonatomic, assign) BOOL debugMode;
@property(nonatomic, assign) BOOL showNetworkActivityIndicator;
@property(nonatomic, strong, nullable) NSString *debugLogFilePath;
@property(nonatomic, strong, nullable) SORIManagerCloudResponseHandler cloudResponseHandler;
@property(nonatomic, readonly) NSString *sessionID;
@property(nonatomic, strong, nullable) SORIManagerRawBufferHandler rawBufferHandler;
@property(nonatomic, strong, nullable) SORIManagerMatchIntervalHandler matchIntervalHandler;
/// Optional explicit model/audio-pack path for compatibility.
/// Normal integrations should leave this unset and use the framework-bundled model.
@property(nonatomic, strong, nullable) NSString *baseModelPath;
@property(nonatomic, strong, nullable) NSDate *nextUpdateDate;

+ (SORIAudioRecognizer *)sharedRecognizer NS_SWIFT_NAME(shared());

/// Applies every property of `configuration`. The recognition mode and server recognition options are stored
/// without validation (this method has no error channel): an invalid pair makes the next fresh start stop with
/// SORIErrorCodeInvalidConfiguration. Prefer `applyConfiguration:error:`.
- (void)configureWithConfiguration:(SORIAudioRecognizerConfiguration *)configuration
    NS_SWIFT_NAME(configure(_:));
/// Validates the recognition mode and server recognition options of `configuration` (with its `audiomarker`) and,
/// when they are valid, applies the configuration as `configureWithConfiguration:` does. Returns NO with
/// SORIErrorCodeInvalidConfiguration and changes nothing when they are invalid. The recognition mode and options
/// reach a running session only at the next `startRecognition...` call; see
/// `-[SORIManager setRecognitionMode:serverRecognition:error:]`.
- (BOOL)applyConfiguration:(SORIAudioRecognizerConfiguration *)configuration error:(NSError *_Nullable *_Nullable)error
    NS_SWIFT_NAME(apply(_:));

- (void)prepare NS_SWIFT_NAME(prepare());
- (void)prepareWithType:(SORIManagerDetectType)type NS_SWIFT_NAME(prepare(type:));
- (void)prepareWithCustomAudioPackPaths:(NSArray<NSString *> *)paths NS_SWIFT_NAME(prepare(customAudioPackPaths:));
- (void)startRecognitionWithRepeat:(BOOL)repeat
                           handler:(SORIManagerRecognitionHandler _Nullable)handler
    NS_SWIFT_NAME(startRecognition(repeat:handler:));
/// Returns the accepted recognition ID. Duplicate starts retain the first call; a duplicate start on a running
/// session applies a pending server recognition tuning change in place (see `setRecognitionMode:serverRecognition:error:`).
/// Callbacks run asynchronously on main. Only stopped is terminal; its error is
/// nil for intentional stop. Compare IDs when handling queued older outcomes.
- (NSString *)startRecognitionWithRepeat:(BOOL)repeat
                                handler:(SORIManagerRecognitionHandler _Nullable)handler
                       lifecycleHandler:(SORIRecognitionLifecycleHandler _Nullable)lifecycleHandler
    NS_SWIFT_NAME(startRecognition(repeat:handler:lifecycleHandler:));
- (void)stopRecognition NS_SWIFT_NAME(stopRecognition());

/// Configures the recognition mode and server recognition options. Same contract as
/// `-[SORIManager setRecognitionMode:serverRecognition:error:]`: validated here, applied by the next
/// `startRecognition...` call, never stops a running session.
- (BOOL)setRecognitionMode:(SORIRecognitionMode)recognitionMode
         serverRecognition:(nullable SORIServerRecognitionOptions *)serverRecognition
                     error:(NSError *_Nullable *_Nullable)error
    NS_SWIFT_NAME(setRecognitionMode(_:serverRecognition:));
- (void)clearState;
- (void)reset;
- (void)updateDatabase NS_SWIFT_NAME(updateDatabase());
- (void)removeAllAudioPacks;
- (void)clearAudioDatabaseWithCompletionHandler:
    (SORIManagerClearIDBCompletionHandler _Nullable)completionHandler
    NS_SWIFT_NAME(clearAudioDatabase(completionHandler:));
- (BOOL)audioRecordingPermitted;

@end

NS_ASSUME_NONNULL_END
