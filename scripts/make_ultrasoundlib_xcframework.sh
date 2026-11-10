#!/usr/bin/env bash
set -euo pipefail

# Builds UltrasoundLib.xcframework with two slices:
#   • ios-arm64                    → the REAL Leltek device framework (dynamic)
#   • ios-arm64_x86_64-simulator   → a simulator-safe STUB framework (empty ObjC
#                                     methods, just enough symbols for the linker)
#
# Mirrors the Sonostar approach, adapted for a DYNAMIC framework instead of a
# static .a:
#   - the stub is built with `-dynamiclib` and assembled into a .framework
#   - the xcframework is created with `-framework` (not `-library`)
#   - in the app it must be Embed & Sign (dynamic), not "Do Not Embed"
#
# Because both slices ship identical headers + modulemap, `import UltrasoundLib`
# resolves on device AND simulator — so the Swift service needs no #if guards.

# Default device framework = the ios-arm64 slice of the committed xcframework, so
# the script is re-runnable without re-supplying the raw Leltek SDK framework.
# Pass --device-framework to point at a fresh framework from the SDK download.
DEVICE_FRAMEWORK="UltrasoundLib.xcframework/ios-arm64/UltrasoundLib.framework"
OUTPUT_DIR="."
NAME="UltrasoundLib"
MIN_IOS_SIM="17.6"   # match the app's iOS Deployment Target
INSTALL_NAME="@rpath/UltrasoundLib.framework/UltrasoundLib"

while [[ $# -gt 0 ]]; do
  case "$1" in
    --device-framework) DEVICE_FRAMEWORK="$2"; shift 2 ;;
    --output-dir)       OUTPUT_DIR="$2"; shift 2 ;;
    --name)             NAME="$2"; shift 2 ;;
    --min-ios)          MIN_IOS_SIM="$2"; shift 2 ;;
    -h|--help) echo "Usage: $0 [--device-framework PATH] [--output-dir DIR] [--name NAME] [--min-ios 17.6]"; exit 0 ;;
    *) echo "Unknown arg: $1"; exit 1 ;;
  esac
done

[[ -d "$DEVICE_FRAMEWORK" ]] || { echo "ERROR: device framework not found: $DEVICE_FRAMEWORK"; exit 1; }
command -v xcrun >/dev/null      || { echo "ERROR: xcrun not found"; exit 1; }
command -v xcodebuild >/dev/null || { echo "ERROR: xcodebuild not found"; exit 1; }

mkdir -p "$OUTPUT_DIR"
WORKDIR="$(mktemp -d "${TMPDIR:-/tmp}/ulxfw.XXXXXX")"
trap 'rm -rf "$WORKDIR"' EXIT

# Copy the device framework out first — its default location is INSIDE the output
# xcframework, which gets removed before the rebuild. Preserve the framework name
# (its binary / CFBundleExecutable must match the bundle directory name).
DEVICE_FRAMEWORK_NAME="$(basename "$DEVICE_FRAMEWORK")"
mkdir -p "$WORKDIR/device"
cp -R "$DEVICE_FRAMEWORK" "$WORKDIR/device/$DEVICE_FRAMEWORK_NAME"
DEVICE_FRAMEWORK="$WORKDIR/device/$DEVICE_FRAMEWORK_NAME"
HEADERS_DIR="$DEVICE_FRAMEWORK/Headers"
MODULES_DIR="$DEVICE_FRAMEWORK/Modules"
[[ -d "$HEADERS_DIR" ]] || { echo "ERROR: headers dir not found in device framework"; exit 1; }

echo "• Workdir: $WORKDIR"
echo "• Simulator minimum iOS: $MIN_IOS_SIM"

# --- 1. Simulator stub source -------------------------------------------------
# Implements only the LibManager methods LeltekService actually calls. The real
# LibManager.h is imported, so signatures are checked against the public API and
# the resulting class exports the same ObjC symbol the device build links.
cat > "$WORKDIR/stub.m" <<'EOF'
#import "LibManager.h"

#pragma clang diagnostic ignored "-Wincomplete-implementation"
#pragma clang diagnostic ignored "-Wobjc-missing-super-calls"

@implementation LibManager

+ (instancetype)shared {
  static LibManager *s; static dispatch_once_t once;
  dispatch_once(&once, ^{ s = [LibManager new]; });
  return s;
}

- (void)set_Delegate:(id)_delegate {}
- (void)removeDelegate {}
- (void)initLib {}
- (void)isFEBEExistsAtPath {}
- (void)connect:(int)force_wifi_channel WiFi5G:(int)force_wifi_channel5G ssid:(NSString *)_ssid enable5GHzSwitchPrompt:(bool)_enable5GHzSwitchPrompt {}

- (void)libStartScan {}
- (void)libStopScan {}

- (void)libSetDepth:(float)fNewDepth {}
- (float)libGetDepth { return 0.0f; }

- (void)libSetParam:(int)mode paramId:(int)pid data:(float)data {}
- (float)libGetParam:(int)mode paramId:(int)pid { return 0.0f; }

- (int)libGetBatteryLevel { return 0; }
- (int)libGetTemperature { return 0; }
- (int)libGetProbeType:(bool)isRealId { return 0; }

- (void)libSwitchToBMode {}
- (void)libSwitchToCMode {}
- (void)libSwitchToMMode {}
- (int)libGetMode { return 0; }

- (NSMutableArray *)getPresetOptions:(int)headIndex { return [NSMutableArray array]; }
- (void)libSetAndReturnSettingParams:(int)sid {}
- (void)libTermination {}

@end
EOF

# --- 2. Compile stub for both simulator archs --------------------------------
echo "• Compiling simulator stub (arm64 + x86_64)…"
for ARCH in arm64 x86_64; do
  xcrun -sdk iphonesimulator clang -fobjc-arc -dynamiclib \
    -target "${ARCH}-apple-ios${MIN_IOS_SIM}-simulator" \
    -install_name "$INSTALL_NAME" \
    -I "$HEADERS_DIR" \
    -framework Foundation -framework UIKit \
    "$WORKDIR/stub.m" -o "$WORKDIR/${NAME}_${ARCH}.dylib"
done

lipo -create "$WORKDIR/${NAME}_arm64.dylib" "$WORKDIR/${NAME}_x86_64.dylib" \
  -output "$WORKDIR/${NAME}"

# --- 3. Assemble the stub .framework -----------------------------------------
STUB_FW="$WORKDIR/${NAME}.framework"
mkdir -p "$STUB_FW/Headers" "$STUB_FW/Modules"
cp "$WORKDIR/${NAME}" "$STUB_FW/${NAME}"
cp "$HEADERS_DIR"/*.h "$STUB_FW/Headers/"
cp "$MODULES_DIR/module.modulemap" "$STUB_FW/Modules/module.modulemap"

cat > "$STUB_FW/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleDevelopmentRegion</key><string>en</string>
  <key>CFBundleExecutable</key><string>${NAME}</string>
  <key>CFBundleIdentifier</key><string>leltek.${NAME}</string>
  <key>CFBundleInfoDictionaryVersion</key><string>6.0</string>
  <key>CFBundleName</key><string>${NAME}</string>
  <key>CFBundlePackageType</key><string>FMWK</string>
  <key>CFBundleShortVersionString</key><string>1.0</string>
  <key>CFBundleVersion</key><string>1</string>
  <key>CFBundleSupportedPlatforms</key><array><string>iPhoneSimulator</string></array>
  <key>MinimumOSVersion</key><string>${MIN_IOS_SIM}</string>
</dict>
</plist>
EOF

# --- 4. Create the xcframework -----------------------------------------------
OUT_XCFW="$OUTPUT_DIR/${NAME}.xcframework"
rm -rf "$OUT_XCFW"
echo "• Creating XCFramework → $OUT_XCFW"
xcodebuild -create-xcframework \
  -framework "$DEVICE_FRAMEWORK" \
  -framework "$STUB_FW" \
  -output "$OUT_XCFW" >/dev/null

echo "• Slices:"
/usr/libexec/PlistBuddy -c "Print :AvailableLibraries" "$OUT_XCFW/Info.plist" 2>/dev/null \
  | grep -E "LibraryIdentifier|SupportedPlatform" || find "$OUT_XCFW" -maxdepth 1 -type d
echo "✅ Done: $OUT_XCFW"
