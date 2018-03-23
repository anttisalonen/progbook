using System;

public class Split {
    public static void Main(string[] args) {
        string str = "This is a string.\n";
        string[] words = str.Split(' ');
        foreach(var s in words) {
            Console.WriteLine(s);
        }
    }
}

