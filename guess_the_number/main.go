package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func Guess(l, r int) int {
	if (r-l)%2 == 0 {
		return (r-l)/2 + l
	} else {
		return (r-l)/2 + 1 + l
	}
}

func process(s string) {
	splts := strings.Split(s, " ")
	if n, err := strconv.Atoi(splts[0]); err != nil {
		panic("initial number convert failed")
	} else {
		l, r := 0, n
		n = Guess(l, r)
		for _, s := range splts[1:] {
			// fmt.Println(l, r, n)
			switch s {
			case "Lower":
				r = n - 1
				n = Guess(l, r)
			case "Higher":
				l = n + 1
				n = Guess(l, r)
			case "Yay!":
				fmt.Println(n)
				return
			}
		}
	}
}

func main() {
	if f, err := os.Open(os.Args[1]); err != nil {
		fmt.Println(err)
		return
	} else {
    defer f.Close()
		sc := bufio.NewScanner(f)
		for sc.Scan() {
			process(sc.Text())
		}
	}
}
