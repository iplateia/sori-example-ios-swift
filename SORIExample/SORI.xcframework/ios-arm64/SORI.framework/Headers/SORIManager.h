/*!
 @author Heeseung Seo <hirenn@iplateia.com>
 @charset utf-8
 @copyright iPlateia Inc.
 */

#import <Foundation/Foundation.h>

/// SORI Manager Detection Type
typedef NS_ENUM(int, SORIManagerDetectType) {
    /// This is not used
    SORIManagerDetectTypeInitial = 0,
    /// Detect using SORI API
    SORIManagerDetectTypeCloud,
    /// Detect using local engine (eg. In-app Recognition)
    SORIManagerDetectTypeLocal,
    /// This is not used for 3rd party
    SORIManagerDetectTypeReserved
};

/// SORI API Version
typedef NS_ENUM(int, SORIAPIVersion) {
    /// SORI API Version 1
    SORIAPIVersionV1 = 1,
    /// SORI API Version 2
    SORIAPIVersionV2,
    /// SORI API Version 3
    SORIAPIVersionV3
};

/// Detemine Recognition Result Structure
typedef NS_ENUM(int, SORIManagerResultType) {
    /// SORI will result with cloud response model structure
    SORIManagerResultTypeCloudCompatible,
    /// SORI will result with simplified reponse model structure (like `android_lib`)
    SORIManagerResultTypeLocal
};

typedef void (^SORIManagerCloudResponseHandler)(NSData *_Nullable data,
                                                NSError *_Nullable error);
typedef void (^SORIManagerRecognitionHandler)(NSDictionary *_Nullable media,
                                              NSError *_Nullable error);
typedef void (^SORIManagerDetectMediaHandler)(NSDictionary *_Nullable media)
    __deprecated;
typedef void (^SORIManagerAudioPackLoadCompletionHandler)(BOOL succeed);
typedef void (^SORIManagerRawBufferHandler)(void *_Nonnull buffer, size_t size);
typedef void (^SORIManagerClearIDBCompletionHandler)(BOOL succeed);
typedef void (^SORIManagerMatchIntervalHandler)(NSTimeInterval interval);

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

/// SORI API Version
@property(nonatomic, assign) SORIAPIVersion apiVersion;

/// SORI results model structure type
@property(nonatomic, assign) SORIManagerResultType resultType;

/// If YES, SORI prepared successfully.
@property(nonatomic, readonly) BOOL prepared;

/// If YES, the SORI running.
@property(nonatomic, readonly) BOOL running;

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

/// If YES, SORI will send user's current location when requesting API Server.
/// This will work when detect type is Cloud.
@property(nonatomic, assign) BOOL useLocationService;

/// You can assign instance of SORIContinuousHitManager class.
@property(nonatomic, strong, nullable) SORIContinuousHitManager *hitManager;

/// If YES, logging SORI internal logs.
/// PS. This will put very huge messages to your log console.
@property(nonatomic, assign) BOOL debugMode;

/// This property was deprecated.
/// If YES, SORIManager will work by old way (V1)
/// You cannot use legacy mode for local detect type.
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

/// Just use to get match interval of each local request. (EXPERIMENTAL)
@property(nonatomic, strong, nullable) SORIManagerMatchIntervalHandler matchIntervalHandler;

/// Barrier audio pack bundle path
@property(nonatomic, strong, nullable) NSString *barrierBundlePath;

/// SORI will updates audiopacks after this time, unless nil.
/// NOTE: Operation time is not guaranteed because it depends on timer interval.
@property(nonatomic, strong, nullable) NSDate *nextUpdateDate;

/// SORIManager singleton factory
+ (SORIManager *_Nonnull)sharedManager;

/// Not documented
- (NSURL *_Nonnull)apiURLWithSuffix:(NSString *_Nonnull)suffix;

/// Prepare SORIManager.
/// This is contains authentication, download audio pack, and etc processes.
/// You cannot call this when useLegacy is YES(true).
/// Do not use this with prepareWithCustomAudioPackPaths method together.
/// @param type
///     SORIManagerDetectType value
- (void)prepareWithType:(SORIManagerDetectType)type;

/// Prepare SORIManager for local(In-app Recognition).
/// You cannot call when useLegacy is YES(true).
/// Do not use this with prepareWithType method together.
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

/// Start SORI Manager. Call prepareWithType or prepareWithCustomAudioPackPaths method before call this method.
/// @param repeat
///     If YES, SORI not stop when detect any media.
/// @param recognitionHandler
///     Calling if detect any media
- (void)startWithRepeat:(BOOL)repeat
     recognitionHandler:
         (SORIManagerRecognitionHandler _Nullable)recognitionHandler;

/// Stop SORI Manager
- (void)stop;

/// Clear last recognition media ID.
/// This allows SORI to detect previous detected media again.
- (void)clearState;

/// Remove all informations and files for SORI.
/// This is not available when SORIManager is running.
/// Please do not call this method if you have any no problem to use SORI.
- (void)reset;

/// Remove local IDB. (It's not recommend to call this without any knowledge about this.)
- (void)clearIDBWithCompletionHandler:
    (SORIManagerClearIDBCompletionHandler _Nullable)completionHandler;

/// logging local informations for detailed logs
- (void)logLocalInformations;

#pragma mark - Permissions

/// This method returns whether the audio device permission has been granted.
- (bool)audioRecordingPermitted;

@end
