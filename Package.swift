// swift-tools-version:4.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "AtariKit",
    products: [
        .library(name: "AtariKit", type: .static, targets: ["AtariKit"])
    ],
    dependencies: [
        .package(url: "https://github.com/Octadero/CALE.git", from: "0.0.2"),
    ],
    targets: [
        .target(name: "AtariKit", dependencies: []),
        .testTarget(name: "AtariKitTests", dependencies: ["AtariKit"]),
    ],
    cLanguageStandard: .c11,
    cxxLanguageStandard: .cxx11
)
