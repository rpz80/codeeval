package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"regexp"
	"sort"
	"strconv"
	"strings"
)

type hole struct {
	w int
	h int
}

type brick struct {
	id int
	x  int
	y  int
	z  int
}

var (
	holeRegex  = regexp.MustCompile(`\[(-?\d+),\s*(-?\d+)\]\s*\[(-?\d+),\s*(-?\d+)\]`)
	brickRegex = regexp.MustCompile(`\s*(\d+)\s*\[(-?\d+),\s*(-?\d+),\s*(-?\d+)\]\s*\[(-?\d+),\s*(-?\d+),\s*(-?\d+)\]`)
)

func stoi(s string) int {
	ret, err := strconv.Atoi(s)
	if err != nil {
		fmt.Println("atoi error")
		return -1
	} else {
		return ret
	}
}

func NewHole(s string) *hole {
	result := holeRegex.FindStringSubmatch(s)
	x1 := stoi(result[1])
	y1 := stoi(result[2])
	x2 := stoi(result[3])
	y2 := stoi(result[4])
	return &hole{
		int(math.Abs(float64(x1 - x2))),
		int(math.Abs(float64(y1 - y2)))}
}

func NewBrick(s string) *brick {
	result := brickRegex.FindStringSubmatch(s)
	id := stoi(result[1])
	x1 := stoi(result[2])
	y1 := stoi(result[3])
	z1 := stoi(result[4])
	x2 := stoi(result[5])
	y2 := stoi(result[6])
	z2 := stoi(result[7])

	return &brick{id,
		int(math.Abs(float64(x1 - x2))),
		int(math.Abs(float64(y1 - y2))),
		int(math.Abs(float64(z1 - z2)))}
}

func checkHole(h *hole, b *brick) bool {
	br := []int{b.x, b.y, b.z}
	result := false
	intermed := make([]int, 0)

	in_ := func(ar []int, v int) bool {
		for i := 0; i < len(ar); i++ {
			if ar[i] == v {
				return true
			}
		}
		return false
	}

	for j := 0; j < len(br); j++ {
		if br[j] <= h.w {
			intermed = append(intermed, j)
			for k := 0; k < len(br); k++ {
				if k != j && br[k] <= h.h {
					result = true
				}
			}
		}
	}

	if !result {
		for j := 0; j < len(br); j++ {
			if !in_(intermed, j) && br[j] <= h.w {
				intermed = append(intermed, j)
				for k := 0; k < len(br); k++ {
					if k != j && br[k] <= h.h {
						result = true
					}
				}
			}
		}
	}

	if !result {
		for j := 0; j < len(br); j++ {
			if !in_(intermed, j) && br[j] <= h.w {
				intermed = append(intermed, j)
				for k := 0; k < len(br); k++ {
					if k != j && br[k] <= h.h {
						result = true
					}
				}
			}
		}
	}

	return result
}

func printResult(r []int) {
	sort.Sort(sort.IntSlice(r))
	if len(r) == 0 {
		fmt.Println("-")
		return
	}
	for i := 0; i < len(r); i++ {
		fmt.Print(r[i])
		if i != len(r)-1 {
			fmt.Print(",")
		}
	}
	fmt.Println("")
}

func Process(s string) {
	splts := strings.Split(s, "|")
	h := NewHole(splts[0])
	brickSplts := strings.Split(splts[1], ";")
	result := make([]int, 0, 16)
	for _, v := range brickSplts {
		b := NewBrick(v)
		// fmt.Println("brick: ", b, "hole: ", h)
		if checkHole(h, b) {
			result = append(result, b.id)
		}
	}
	printResult(result)
}

func main() {
	defer func() {
		if r := recover(); r != nil {
			fmt.Printf("Something gone terribly wrong: %s", r)
		}
	}()
	if len(os.Args) < 2 {
		fmt.Print("Usage: biw <file-name>\n")
		return
	}
	if f, err := os.Open(os.Args[len(os.Args)-1]); err != nil {
		fmt.Println("File open error")
		return
	} else {
		sc := bufio.NewScanner(f)
		for sc.Scan() {
			Process(sc.Text())
		}
	}
}
