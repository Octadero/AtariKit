// swift-tools-version:4.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "AtariKit",
    products: [
        .library(name: "ALE", targets: ["ALE"]),
        .library(name: "AtariKit", targets: ["AtariKit"]),
    ],
    dependencies: [
        .package(url: "https://github.com/Octadero/CALE.git", from: "0.0.1"),
    ],
    targets: [
        .target(name: "ALE", dependencies: []),
        .target(name: "AtariKit", dependencies: ["ALE"]),
        .testTarget(name: "AtariKitTests", dependencies: ["AtariKit"]),
    ]
)
