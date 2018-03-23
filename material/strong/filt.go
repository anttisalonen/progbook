package main

import (
	"fmt"
)

func main() {
    d := []int {3, -1, 4, -2, 5, -3, 6, -4}
    d2 := filt(d)
	for _, v := range d2 {
		fmt.Println(v)
	}
}

func filt(arr []int) (ret []int) {
    for _, i := range arr {
        if i < 0 {
            ret = append(ret, i)
        }
    }
    return
}
