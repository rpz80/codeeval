package main

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"time"
)

func calc(vals ...int) bool {
	if len(vals) == 2 {
		if vals[0]+vals[1] == 42 {
			return true
		}

		if vals[0]-vals[1] == 42 {
			return true
		}

		if vals[0]*vals[1] == 42 {
			return true
		}
		return false
	} else if len(vals) == 4 {
		return calc(vals[0]+vals[1], vals[2], vals[3]) ||
			calc(vals[0]-vals[1], vals[2], vals[3]) ||
			calc(vals[0]*vals[1], vals[2], vals[3])
	} else if len(vals) == 3 {
		return calc(vals[0]+vals[1], vals[2]) ||
			calc(vals[0]-vals[1], vals[2]) ||
			calc(vals[0]*vals[1], vals[2])
	}
	return false
}

func process(s string) {
	splts := strings.Split(s, " ")
	cards := make([]int, 0, len(splts))

	for _, v := range splts {
		i, err := strconv.ParseInt(v, 10, 32)
		if err != nil {
			fmt.Printf("Error converting %s to int\n", v)
			os.Exit(1)
		}
		cards = append(cards, int(i))
	}

	var permCount = 0
	found := false

	for i := 0; i < len(cards); i++ {
		for j := 0; j < len(cards); j++ {
			for k := 0; k < len(cards); k++ {
				for l := 0; l < len(cards); l++ {
					for m := 0; m < len(cards); m++ {
						if i != j && k != j && k != i && m != i && m != j &&
							m != k && m != l && l != i && l != j && l != k {
							// fmt.Printf("%3v %3v %3v %3v %3v\n", cards[i],
							// 	cards[j], cards[k], cards[l], cards[m])
							permCount++

							found = calc(cards[i]+cards[j], cards[k], cards[l], cards[m])
							if found {
								fmt.Println("YES")
								return
							}

							found = calc(cards[i]*cards[j], cards[k], cards[l], cards[m])
							if found {
								fmt.Println("YES")
								return
							}

							found = calc(cards[i]-cards[j], cards[k], cards[l], cards[m])
							if found {
								fmt.Println("YES")
								return
							}
						}
					}
				}
			}
		}
	}

	fmt.Println("NO")
}

func main() {
	t0 := time.Now()

	if len(os.Args) < 2 {
		fmt.Printf("Usage: %s <fname>\n", filepath.Base(os.Args[0]))
		os.Exit(1)
	}

	f, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Printf("File open error: %s\n", os.Args[1])
		os.Exit(1)
	}

	defer f.Close()

	sc := bufio.NewScanner(f)
	for sc.Scan() {
		process(sc.Text())
	}

	t1 := time.Now()
	fmt.Printf("elapsed: %v", t1.Sub(t0))
}
