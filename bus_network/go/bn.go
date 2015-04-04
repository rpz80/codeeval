package main

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
	"regexp"
	"strconv"
	"strings"
)

var (
	points_re = regexp.MustCompile(`\((\d+),\s*(\d+)\)`)
	stops_re  = regexp.MustCompile(`R\d+=\[(.+)\]`)
)

type node struct {
	route   uint8
	label   uint
	visited bool
	childs  []*node
}

type graph struct {
	first *node
}

func newNode(route uint8, label uint) *node {
	return &node{
		route:   route,
		label:   label,
		visited: false,
		childs:  make([]*node, 0)}
}

func newGraph() graph {
	return graph{nil}
}

func (g *graph) appendRoute(route uint8, stops []uint) {
	if g.first == nil {
		g.first = newNode(route, stops[0])
		stops = stops[1:]
	}

}

func process(s string) {
	splts := strings.Split(s, " ")

	points_match := points_re.FindStringSubmatch(splts[0])
	if len(points_match) != 3 {
		return
	}
	start, stop := points_match[1], points_match[2]

	g := newGraph()

	for i := 1; i < len(splts); i++ {
		stops_match := stops_re.FindStringSubmatch(splts[i])
		if len(stops_match) != 2 {
			fmt.Errorf("Stops regex failed\n")
			return
		}

		stops := strings.Split(stops_match[1], ",")
		stops_int := make([]uint, 0, len(stops))

		for _, v := range stops {
			si, err := strconv.ParseUint(v, 10, 32)
			if err != nil {
				fmt.Errorf("Parse stop failed\n")
				return
			}
			stops_int = append(stops_int, uint(si))
		}
		g.appendRoute(uint8(i), stops_int)
	}
	fmt.Println(start, stop)
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println(
			fmt.Sprintf("Usage: %s <file-name>\n", filepath.Base(os.Args[0])))
		return
	}

	f, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Println("File open error")
		return
	}

	sc := bufio.NewScanner(f)
	for sc.Scan() {
		process(sc.Text())
	}
}
