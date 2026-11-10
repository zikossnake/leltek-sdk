# Leltek SDK

The Leltek `UltrasoundLib.xcframework` and the kernel resources it reads at runtime,
packaged for Swift Package Manager so the
[deepecho-mobile-app](https://github.com/Deepecho-org/deepecho-mobile-app) does not
carry a vendor binary in its own history.

## Contents

| Path | What it is |
| --- | --- |
| `UltrasoundLib.xcframework/ios-arm64/UltrasoundLib.framework` | The real Leltek SDK (dynamic framework), from the vendor. Device only. |
| `UltrasoundLib.xcframework/ios-arm64_x86_64-simulator/UltrasoundLib.framework` | A hand-written no-op stub. Implements only the `LibManager` methods the app calls. |
| `Resources/FEBE.db` | The kernel's probe/preset database. |
| `Resources/cfg/` | Per-probe init tables, colour maps, wall filters, FPGA and Wi-Fi firmware. |
| `scripts/make_ultrasoundlib_xcframework.sh` | Rebuilds the XCFramework from a vendor `UltrasoundLib.framework`. |

**The binary and the resources are one release.** The kernel reads `FEBE.db`'s tables and
the `cfg/` files of its own version; never update one without the other.

**The simulator slice is a stub.** It exists only so the app links and runs in the
Simulator; every call through it does nothing. A green simulator build says nothing about
whether Leltek works — that requires a physical probe and a device build.

## Versions

| Tag | Leltek kernel | Leltek source |
| --- | --- | --- |
| `1.0.0` | 1.26.1.102 (UI_911d51dd2, kernel_67b55c35), Leltek app 7.1.39 | `gitlab.com/lelsdk-s024/lelsdk_ios_s024` @ `051c575d` (2026-10-30) |

## Consuming it

```swift
// Tuist/Package.swift
.package(url: "https://github.com/zikossnake/leltek-sdk.git", exact: "1.0.0"),
```

```swift
// Project.swift
.external(name: "UltrasoundLib"),
```

The library is dynamic: the app must embed and sign it (Tuist does this for `.external`).

Pin with `exact:`. The app's `Tuist/Package.resolved` is gitignored, so a version range
would let different machines link different SDK builds without anyone noticing.

## Resources

The SDK looks for `FEBE.db` and `cfg/` at the root of the **main bundle**
(`[NSBundle mainBundle].resourcePath`) and has no API to point it elsewhere. SPM resources
would land in a sub-bundle, so this package declares none. The app copies both from its
checkout of this package instead:

```swift
// Project.swift resources
"Tuist/.build/checkouts/leltek-sdk/Resources/FEBE.db",
.folderReference(path: "Tuist/.build/checkouts/leltek-sdk/Resources/cfg"),
```

## Updating to a new vendor SDK

1. Take the vendor's `Library/frameworks/UltrasoundLib.framework` and `Resources/`.
2. Rebuild the XCFramework:

   ```bash
   ./scripts/make_ultrasoundlib_xcframework.sh \
       --device-framework <vendor>/Library/frameworks/UltrasoundLib.framework
   ```

   If a method the app calls changed, update the stub in the script first.
3. Replace `Resources/FEBE.db` and `Resources/cfg/` with the vendor's, wholesale.
4. Add a row to **Versions**, commit, tag, and bump the `exact:` pin in the app.
