import Foundation

func filt<T: BinaryInteger>(arr: [T]) -> [T] {
    return arr.filter { $0 < 0 }
}

let d = [3, -1, 4, -2, 5, -3, 6, -4]
let d2 = filt(arr: d)

for i in d2 {
    print(i)
}

