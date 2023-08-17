//
//  ContentView.swift
//  SORIExample
//
//  Created by Heeseung Seo on 2023/08/16.
//

import SwiftUI
import SORI

struct ContentView: View {
    @State private var buttonTitle = "Start"
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
                Button(buttonTitle) {
                    if SORIManager.shared().running {
                        stopSORI()
                        buttonTitle = "Start"
                    } else {
                        startSORI()
                        buttonTitle = "Stop"
                    }
                }
            }
        }
    }
    
    func startSORI() {
        SORIManager.shared().applicationID = "Your Application ID"
        SORIManager.shared().secretKey = "Your Secret Key"
        SORIManager.shared().prepare(with: .local)  // .local means "In App Recognition"
        SORIManager.shared().start(withRepeat: true, recognitionHandler: {
            (media, error) in
            if let error = error {
                print("Error: \(error.localizedDescription)")
            } else {
                self.onRecognized(mediaJSON: media as? [String : Any] ?? [:])
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
        SORIManager.shared().stop()
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
