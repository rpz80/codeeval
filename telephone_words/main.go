package main

import (
	"bufio"
	"errors"
	"fmt"
	"os"
	"sort"
	"strings"
)

func convert(b byte) (byte, error) {
	switch b {
	case '2':
		return 'a', nil
	case 'a':
		return 'b', nil
	case 'b':
		return 'c', nil
	case 'c', 'f', 'i', 'l', 'o', 's', 'v', 'z':
		return 1, errors.New("can't convert")
	case '3':
		return 'd', nil
	case 'd':
		return 'e', nil
	case 'e':
		return 'f', nil
	case '4':
		return 'g', nil
	case 'g':
		return 'h', nil
	case 'h':
		return 'i', nil
	case '5':
		return 'j', nil
	case 'j':
		return 'k', nil
	case 'k':
		return 'l', nil
	case '6':
		return 'm', nil
	case 'm':
		return 'n', nil
	case 'n':
		return 'o', nil
	case '7':
		return 'p', nil
	case 'p':
		return 'q', nil
	case 'q':
		return 'r', nil
	case 'r':
		return 's', nil
	case '8':
		return 't', nil
	case 't':
		return 'u', nil
	case 'u':
		return 'v', nil
	case '9':
		return 'w', nil
	case 'w':
		return 'x', nil
	case 'x':
		return 'y', nil
	case 'y':
		return 'z', nil
	}
	return 1, errors.New("unknown byte")
}

func _in(s string, ar []string) bool {
	for _, v := range ar {
		if s == v {
			return true
		}
	}
	return false
}

func doProcess(bs []byte, pos int, result *[]string) {
	if pos >= len(bs) {
		if !_in(string(bs), *result) {
			*result = append(*result, string(bs))
		}
		return
	}
	if bs[pos] == '1' || bs[pos] == '0' {
		doProcess(bs, pos+1, result)
	} else {
		if bconv, err := convert(bs[pos]); err != nil {
			doProcess(bs, pos+1, result)
		} else {
			bs[pos] = bconv
			bs_cp := make([]byte, len(bs))
			copy(bs_cp, bs)
			doProcess(bs, pos, result)
			doProcess(bs_cp, pos+1, result)
		}
	}
}

func process(s string) {
	result := make([]string, 0, 256)
	if s[0] == '1' || s[0] == '0' {
		doProcess([]byte(s), 1, &result)
	} else {
		bs := []byte(s)
		if bconv, err := convert(bs[0]); err != nil {
			doProcess(bs, 1, &result)
		} else {
			bs[0] = bconv
			bs_cp := make([]byte, len(bs))
			copy(bs_cp, bs)
			doProcess(bs, 0, &result)
			doProcess(bs_cp, 1, &result)
		}
	}
	sort.Sort(sort.StringSlice(result))
	fmt.Println(strings.Join(result, ","))
}

func main() {
	defer func() {
		if err := recover(); err != nil {
			fmt.Println("Fatal: ", err)
		}
	}()
	if len(os.Args) < 2 {
		fmt.Println("Usage: tw <file-name>")
		return
	}
	if f, err := os.Open(os.Args[len(os.Args)-1]); err != nil {
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
