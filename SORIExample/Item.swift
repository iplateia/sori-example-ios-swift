//
//  Item.swift
//  SORIExample
//
//  Created by Heeseung Seo on 2023/08/16.
//

import Foundation

struct Item: Identifiable, Hashable {
    let id: String
    let title: String
    let url: String?
    let image: String
}
