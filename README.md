# SORI Example

This is a sample source code of how to implement SORI SDK in your project in Swift with SwiftUI.
You can use this example to build your own audio recognition service from scratch.

## How to build

Request your App ID and Secret Key from [SORI Console Site](https://console.soriapi.com/account/application/).
Then, put your App ID and Secret Key in `startSORI` method from `ContentView.swift` file.

```swift
SORIManager.shared().applicationID = "App ID"
SORIManager.shared().secretKey = "Secret Key"
```

If you want to debug on a specific device or want to build a release version you need to set your team signing.
