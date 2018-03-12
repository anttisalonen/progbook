extern crate num;
use num::Num;

fn main() {
    let d = [3, -1, 4, -2, 5, -3, 6, -4];
    let d2 = filt(&d);
    for i in d2 {
        println!("{}", i);
    }
}

fn filt<T: Num + PartialOrd + Copy>(arr: &[T]) -> Vec<T> {
    arr.into_iter().filter(|&&i| i < T::zero()).cloned().collect()
}
