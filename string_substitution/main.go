package main

import (
	"container/list"
	"fmt"
	// "reflect"
	"bufio"
	"os"
	"strings"
)

const (
	ORIG = iota
	SUB
	OLD
)

type Word struct {
	s     string
	state int
}

func doReplacements(s, sub, pattern string) *list.List {
	l := list.New()
	s1 := s
	prev := 0
	// fmt.Printf("s1 = %s, sub = %s, pat = %s\n", s1, sub, pattern)
	ind := strings.Index(s1, pattern)
	for ; ind != -1; ind = strings.Index(s1, pattern) {
		prev = 0
		// fmt.Println("ind: ", ind)
		l.PushBack(Word{s1[prev:ind], ORIG})
		l.PushBack(Word{sub, SUB})
		prev = ind + len(pattern)
		s1 = s1[prev:]
		// fmt.Println("s1: ", s1)
	}
	if len(s1) != 0 {
		l.PushBack(Word{s1, ORIG})
	}
	// fmt.Println("l: ", l)
	return l
}

func doReplacementsList(in *list.List, sub, pattern string) {
	for e := in.Front(); e != nil; e = e.Next() {
		w := e.Value.(Word)
		if w.state == ORIG {
			l := doReplacements(w.s, sub, pattern)
			for e2 := l.Front(); e2 != nil; e2 = e2.Next() {
				in.InsertBefore(e2.Value.(Word), e)
				w.state = OLD
				e.Value = w
			}
		}
	}
}

func PrintList(l *list.List) {
	for e := l.Front(); e != nil; e = e.Next() {
		fmt.Print(e.Value.(Word))
	}
	fmt.Println("")
}

func process(s string) {
	splts := strings.Split(s, ";")
	l := list.New()
	l.PushBack(Word{splts[0], ORIG})
	splts = strings.Split(splts[1], ",")
	for i := 0; i < len(splts); i += 2 {
		doReplacementsList(l, splts[i+1], splts[i])
	}
	for e := l.Front(); e != nil; e = e.Next() {
		if e.Value.(Word).state != OLD {
			fmt.Print(e.Value.(Word).s)
		}
	}
	fmt.Println("")
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
