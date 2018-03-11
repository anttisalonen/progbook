using System;
using System.Linq;

public class Filt {
    public static void Main(string[] args) {
        int[] d = {3, -1, 4, -2, 5, -3, 6, -4};
        int[] d2 = filt(d);
        foreach(var v in d2) {
            Console.WriteLine(v);
        }
    }

    public static int[] filt(int[] arr) {
        return arr.Where(x => x < 0).ToArray();
    }
}

