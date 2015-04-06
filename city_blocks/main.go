package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const debug = false

type Point struct {
	x float64
	y float64
}

type Rect struct {
	p1 Point
	p2 Point
}

func IfIntersect(a float64, r *Rect) bool {
	if r.p1.x == 0 && r.p1.y == 0 {
		return true
	} else {
		x1 := r.p1.y / a
		if x1 >= r.p1.x && x1 < r.p2.x {
			return true
		}
		y1 := r.p1.x * a
		if y1 >= r.p1.y && y1 < r.p2.y {
			return true
		}
	}
	return false
}

func _atof(s string) float64 {
	if n, err := strconv.ParseInt(s, 10, 32); err != nil {
		panic("convert failed")
	} else {
		return float64(n)
	}
}

func Process(s string) {
	splts := strings.Split(s, " ")
	r := strings.NewReplacer("(", "", ")", "")
	streets := strings.Split(r.Replace(splts[0]), ",")
	avenues := strings.Split(r.Replace(splts[1]), ",")
	a := _atof(avenues[len(avenues)-1]) / _atof(streets[len(streets)-1])
	rects := make([]Rect, 0, 32)
	for i := 0; i < len(streets)-1; i++ {
		for j := 0; j < len(avenues)-1; j++ {
			rects = append(rects, Rect{Point{_atof(streets[i]), _atof(avenues[j])},
				Point{_atof(streets[i+1]), _atof(avenues[j+1])}})
		}
	}
	if debug {
		fmt.Println(rects, a)
	}
	count := 0
	for _, r := range rects {
		if IfIntersect(a, &r) {
			count++
			if debug {
				fmt.Println("success: ", r)
			}
		}
	}
	fmt.Println(count)
}

func main() {
	if f, err := os.Open(os.Args[1]); err != nil {
		fmt.Println(err)
		return
	} else {
    defer f.Close()
		sc := bufio.NewScanner(f)
		for sc.Scan() {
			Process(sc.Text())
		}
	}
}
