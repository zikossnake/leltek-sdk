#pragma once
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, LCLogLevel) {
    LCLogLevelVerbose = 0,
    LCLogLevelDebug,
    LCLogLevelInfo,
    LCLogLevelWarn,
    LCLogLevelError,
    LCLogLevelFatal
};

typedef NS_OPTIONS(NSUInteger, LCLogSink) {
    LCLogSinkConsole = 1 << 0,
    LCLogSinkFile    = 1 << 1
};

/*
 * Thin Objective-C wrapper around the C `logcore` API. Every method is a
 * straight pass-through; all real state (handles, queues, encryption keys)
 * lives in the C core. Both `Log` and `KernelLog` (Swift wrappers) hold
 * their own LogHandle as `void*` and route everything through the
 * per-handle methods below.
 */
@interface LogCoreBridge : NSObject

/* Build + initialize a logger instance. Caller owns the returned handle. */
+ (void* _Nullable)createConfiguredLoggerWithLogDir:(NSString*)logDir
                                         filePrefix:(NSString*)filePrefix
                                            maxDays:(int)maxDays
                                       maxFileBytes:(long)maxFileBytes
                                           minLevel:(LCLogLevel)minLevel
                                              sinks:(LCLogSink)sinks
                                               json:(BOOL)json
                                          uploadURL:(nullable NSString*)uploadURL
                                  uploadIntervalSec:(int)uploadIntervalSec
                                    compressRotated:(BOOL)compressRotated
                                encryptPublicKeyHex:(nullable NSString*)encryptPublicKeyHex;

/* Per-handle ops. `handle` must come from createConfiguredLogger… above. */
+ (void)logToHandle:(void*)handle
              level:(LCLogLevel)level
                tag:(nullable NSString*)tag
          className:(NSString*)cls
       functionName:(NSString*)func
               line:(int)line
                tid:(int64_t)tid
            message:(NSString*)msg;
+ (void)flushHandle:(void*)handle;
+ (void)uploadNowHandle:(void*)handle;
+ (void)dumpRingBufferHandle:(void*)handle;
+ (int64_t)droppedCountHandle:(void*)handle;
+ (void)setMinLevel:(LCLogLevel)level forHandle:(void*)handle;
+ (LCLogLevel)minLevelForHandle:(void*)handle;
+ (BOOL)exportBundleFromHandle:(void*)handle
                        toPath:(NSString*)path
                        format:(int)format
                      password:(nullable NSString*)password;
+ (void)destroyHandle:(void*)handle;

/* Process-wide. installCrashHandlers stores `handle` for the signal /
 * NSException handlers to flush + dump + upload. Only the last caller
 * wins (typically Log; KernelLog inherits via the same handle if it
 * shares, otherwise call again with KernelLog's handle to switch). */
+ (void)installCrashHandlersForHandle:(void*)handle;
+ (int64_t)currentTid;

/* ── App-level bootstrap (project integration glue) ──────────────────────
 * One-stop call from AppDelegate.m: creates the default logger
 * (debug: Documents/logs; release: original
 * Documents/.LELTEK/logs, plaintext, daily rotation, `maxDays` retention),
 * installs crash handlers, and notifies the kernel C++ layer's
 * `kernel_log_set_handle(...)` so LOGD/LOGI/LOGE writes route through
 * logcore. Idempotent. Returns the underlying LogHandle as `void*`
 * (also retrievable via `+sharedHandle` afterwards). */
+ (void* _Nullable)bootstrapDefaultWithMaxDays:(int)maxDays;

/* Returns the handle created by `+bootstrapDefaultWithMaxDays:`, or NULL
 * if bootstrap has not yet been called. Stable for the lifetime of the
 * process. */
+ (void* _Nullable)sharedHandle;
+ (nullable NSString*)sharedLogDirectoryPath;
+ (BOOL)isFileLoggingEnabled;
+ (void)setFileLoggingEnabled:(BOOL)enabled;
+ (BOOL)exportSharedLogBundleToPath:(NSString*)path;

@end

NS_ASSUME_NONNULL_END
