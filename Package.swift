// swift-tools-version:5.9
import PackageDescription

let package = Package(
    name: "LeltekSDK",
    products: [
        .library(name: "UltrasoundLib", targets: ["UltrasoundLib"]),
    ],
    targets: [
        .binaryTarget(name: "UltrasoundLib", path: "UltrasoundLib.xcframework"),
    ]
)
