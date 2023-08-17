//
//  ItemView.swift
//  SORIExample
//
//  Created by Heeseung Seo on 2023/08/16.
//

import SwiftUI

struct ItemView: View {
    let item: Item
    
    var body: some View {
        HStack {
            AsyncImage(url: URL(string: item.image)) { image in
                image.resizable()
                    .aspectRatio(contentMode: .fill)
            } placeholder: {
                ProgressView()
            }
            .frame(width: 64, height: 64)
            .padding()
            Text(item.title)
        }
    }
}

struct ItemView_Previews: PreviewProvider {
    static var previews: some View {
        ItemView(item: Item(id: "1", title: "Title", url: nil, image: "https://placehold.co/600x400.png"))
    }
}
