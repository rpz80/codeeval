package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
	"unicode"
)

var spaceRe = regexp.MustCompile(`\s+`)

func process(s string) {
	var sum int64 = 0
	s = spaceRe.ReplaceAllString(s, "")
	for i := len(s) - 1; i >= 0; i-- {
		if unicode.IsDigit(rune(s[i])) {
			if tmp, err := strconv.ParseInt(string(s[i]), 10, 32); err != nil {
				fmt.Println(err)
				return
			} else {
				if len(s)%2 != 0 {
					if i%2 != 0 {
						tmp *= 2
					}
				} else {
					if i%2 == 0 {
						tmp *= 2
					}
				}
				if tmp >= 10 {
					tmp = 1 + (tmp % 10)
				}
				sum += tmp
			}
		}
	}
	if sum%10 == 0 {
		fmt.Println(1)
	} else {
		fmt.Println(0)
	}
}

func main() {
	defer func() {
		if err := recover(); err != nil {
			fmt.Println("Fatal: ", err)
		}
	}()
	if len(os.Args) < 2 {
		fmt.Println("Usage: cn 'file name'")
		return
	}
	if f, err := os.Open(os.Args[1]); err != nil {
		fmt.Println("File open error")
		return
	} else {
    defer f.Close()
		sc := bufio.NewScanner(f)
		for sc.Scan() {
			process(sc.Text())
		}
	}
}
