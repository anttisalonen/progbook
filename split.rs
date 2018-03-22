fn main() {
    let str = "this is a string.\n";
    let s2 = str.split(" ");
    for word in s2 {
        println!("{}", word);
    }
}
