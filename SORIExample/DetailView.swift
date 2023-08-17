//
//  DetailView.swift
//  SORIExample
//
//  Created by Heeseung Seo on 2023/08/16.
//

import SwiftUI
import WebKit

struct WebView: UIViewRepresentable {

    let url: URL

    func makeUIView(context: Context) -> WKWebView {
        let webview = WKWebView()
        webview.load(URLRequest(url: url))
        return webview
    }
    
    func updateUIView(_ uiView: WKWebView, context: UIViewRepresentableContext<WebView>) {
        
    }
}

struct DetailView: View {
    let urlString: String
    var body: some View {
        WebView(url: URL(string: urlString)!)
    }
    
    
}
