# SORI Example

This is a sample source code of how to implement SORI SDK in your project in Swift with SwiftUI.
You can use this example to build your own audio recognition service from scratch.

## How to build

1. Request your App ID and Secret Key from the [SORI Console](https://console.soriapi.com/account/application/).
2. Create your local credential file from the tracked template:

   ```sh
   cp SORIExample/SORICredentials.swift.example SORIExample/SORICredentials.swift
   ```

3. Open `SORIExample/SORICredentials.swift` and replace both placeholders with your credentials:

```swift
enum SORICredentials {
    static let applicationID = "YOUR_SORI_APPLICATION_ID"
    static let secretKey = "YOUR_SORI_SECRET_KEY"
}
```

4. Open `SORIExample.xcodeproj` in Xcode and build the `SORIExample` scheme. The local credential file is compiled by the target in both Debug and Release configurations. If the file is missing, the build fails and reports `SORIExample/SORICredentials.swift` as the missing input.

If you want to debug on a specific device or build a release version, set your development team under Signing & Capabilities.

## Credential security

`SORIExample/SORICredentials.swift` is intentionally ignored by the repository. Keep it untracked and do not commit it to any repository, including a private repository. The tracked `.example` file is the stable setup template and contains placeholders only; `ContentView.swift` remains ordinary tracked application source.

This local credential seam is intended for development and evaluation. The SDK authentication API requires the application to supply the credential, so this sample uses Swift string literals to keep the integration buildable. Ignoring the source file prevents accidental source-control exposure, but it does not protect the compiled value; build-time injection has the same limitation after the value is packaged. Client-side hardening can raise the extraction cost, but it cannot make a packaged credential perfectly secret.

Before distributing an app, review its credential packaging against the [EULA](./EULA.md) and your threat model rather than shipping this sample setup unchanged. Apply platform-appropriate hardening or obfuscation, limit unnecessary exposure, and be prepared to rotate a key if it is recovered. If your threat model requires server-mediated or short-lived provisioning, confirm a supported production architecture with iPlateia; this sample does not implement or promise such a mechanism.

The ignore rule protects a new, untracked local credential file from a normal `git add .`. It cannot protect a file that was already tracked or added with `git add -f`.

If a Secret Key is exposed, rotate the App Key in the SORI Console immediately and replace the value in your local credential file. Removing the key from a later commit does not make the exposed key safe again.

Run `scripts/validate-credential-seam.sh` to verify the template contract, the exact ignore rule, and that a normal staging operation excludes a unique test credential.
