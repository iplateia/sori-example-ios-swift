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
/// If YES, local recognition also runs near-ultrasonic audiomarker detection.
/// Defaults to NO so recognition remains fingerprint-only unless explicitly enabled.
@property(nonatomic, assign) BOOL audiomarker;
/// Called when the current near-ultrasonic audiomarker changes during local recognition.
/// This handler can receive nil when marker recognition clears.
@property(nonatomic, copy, nullable) SORIManagerAudiomarkerChangeHandler audiomarkerChangeHandler;
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
@property(nonatomic, readonly) BOOL isRecorderRunning;
@property(nonatomic, assign) double cutoff;
@property(nonatomic, assign) BOOL useCutoffFilter;
@property(nonatomic, assign) BOOL useLocationService;
/// If YES, local recognition also runs near-ultrasonic audiomarker detection.
/// Defaults to NO so recognition remains fingerprint-only unless explicitly enabled.
@property(nonatomic, assign) BOOL audiomarker;
/// Called when the current near-ultrasonic audiomarker changes during local recognition.
/// This handler can receive nil when marker recognition clears.
@property(nonatomic, copy, nullable) SORIManagerAudiomarkerChangeHandler audiomarkerChangeHandler;
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

- (void)configureWithConfiguration:(SORIAudioRecognizerConfiguration *)configuration
    NS_SWIFT_NAME(configure(_:));

- (void)prepare NS_SWIFT_NAME(prepare());
- (void)prepareWithType:(SORIManagerDetectType)type NS_SWIFT_NAME(prepare(type:));
- (void)prepareWithCustomAudioPackPaths:(NSArray<NSString *> *)paths NS_SWIFT_NAME(prepare(customAudioPackPaths:));
- (void)startRecognitionWithRepeat:(BOOL)repeat
                           handler:(SORIManagerRecognitionHandler _Nullable)handler
    NS_SWIFT_NAME(startRecognition(repeat:handler:));
- (void)stopRecognition NS_SWIFT_NAME(stopRecognition());
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
