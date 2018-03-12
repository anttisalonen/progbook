import Foundation

let str = "this is a string.\n"
let s2  = str.components(separatedBy: " ")

for word in s2 {
    print(word)
}

