package main

import (
	"fmt"
	"strings"
)

func main() {
	s := "This is a string.\n"
	s2 := strings.Split(s, " ")
	for _, word := range s2 {
		fmt.Println(word)
	}
}
