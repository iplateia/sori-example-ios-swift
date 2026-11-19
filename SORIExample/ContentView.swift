//
//  ContentView.swift
//  SORIExample
//
//  Created by Heeseung Seo on 2023/08/16.
//

import SwiftUI
import SORI

struct ContentView: View {
    /// The recognition session this view started, until the SDK reports it stopped.
    @State private var activeSessionID: String?
    @State private var items = [Item]()
    
    var body: some View {
        NavigationView {
            VStack {
                List(items, id: \.self) { item in
                    NavigationLink {
                        DetailView(urlString: item.url ?? "https://naver.com")
                    } label: {
                        ItemView(item: item)
                    }
                }
            }
            .navigationBarTitle("SORI Example")
            .toolbar {
                Button("Reset") {
                    SORIAudioRecognizer.shared().clearState()
                    items.removeAll()
                }
                Button(activeSessionID == nil ? "Start" : "Stop") {
                    if activeSessionID == nil {
                        startSORI()
                    } else {
                        stopSORI()
                    }
                }
            }
        }
    }
    
    func startSORI() {
        let recognizer = SORIAudioRecognizer.shared()
        let configuration = SORIAudioRecognizer.Configuration(
            applicationID: SORICredentials.applicationID,
            secretKey: SORICredentials.secretKey
        )
        configuration.useLocationService = false

        recognizer.configure(configuration)
        activeSessionID = recognizer.startRecognition(repeat: true, handler: {
            (media, error) in
            if let error = error {
                print("Error: \(error.localizedDescription)")
            } else {
                self.onRecognized(mediaJSON: media as? [String : Any] ?? [:])
            }
        }, lifecycleHandler: { sessionID, state, error in
            // The SDK can end a session on its own: a failed start, a denied
            // microphone permission, or a start that was still pending when the
            // app moved to the background. Follow its report so the button
            // always matches the real state.
            guard state == .stopped else { return }
            if let error = error {
                print("Recognition stopped: \(error.localizedDescription)")
            }
            DispatchQueue.main.async {
                if self.activeSessionID == sessionID {
                    self.activeSessionID = nil
                }
            }
        })
    }
    
    func onRecognized(mediaJSON: [String : Any]) {
        guard let campaign = mediaJSON["campaign"] as? [String : Any] else {
            print("Error: cannot found campaign field")
            return
        }
        
        let item = Item(
            id: mediaJSON["_id"] as! String,
            title: campaign["name"] as? String ?? "No Title",
            url: campaign["action_url"] as? String,
            image: campaign["image"] as? String ?? "https://placehold.co/600x400.png"
        )
        
        for index in items.indices {
            if items[index].id == item.id {
                items.remove(at: index)
                break
            }
        }

        items.insert(item, at: 0)
    }

    func stopSORI() {
        SORIAudioRecognizer.shared().stopRecognition()
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
