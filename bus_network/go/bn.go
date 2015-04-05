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

const (
	max_dist = 100000
)

type node struct {
	route   uint8
	label   uint
	dist    uint
	visited bool
	childs  []*node
}

type graph struct {
	nodes []*node
}

func newNode(route uint8, label uint) *node {
	return &node{
		route:   route,
		label:   label,
		dist:    max_dist,
		visited: false,
		childs:  make([]*node, 0)}
}

func newGraph() graph {
	return graph{make([]*node, 0)}
}

func (g *graph) appendRoute(route uint8, stops []uint) {
	for _, v := range stops {
		g.nodes = append(g.nodes, newNode(route, v))
	}
}

func (g *graph) print() {
	for _, v := range g.nodes {
		fmt.Println(*v)
	}
}

func (g *graph) connectNodes() {
	for i, _ := range g.nodes {

	}
}

func (g *graph) reset() {
	for i, _ := range g.nodes {
		g.nodes[i].dist = max_dist
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
	g.print()
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
