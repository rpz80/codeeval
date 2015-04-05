package main

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
	"regexp"
	"strconv"
	"strings"
	"time"
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
	index   uint
	visited bool
}

type adjacency_matrix struct {
	table []int
	size  uint
}

func newAdjMatrix(size uint) adjacency_matrix {
	return adjacency_matrix{
		make([]int, size*size),
		size}
}

func (m *adjacency_matrix) at(row, col uint) *int {
	return &m.table[row*m.size+col]
}

type graph struct {
	nodes  []*node
	matrix adjacency_matrix
}

type resultStruct struct {
	index  int
	result uint
}

func newNode(route uint8, label uint) *node {
	return &node{
		route:   route,
		label:   label,
		dist:    max_dist,
		visited: false}
}

func newGraph() graph {
	return graph{nodes: make([]*node, 0)}
}

func (g *graph) appendRoute(route uint8, stops []uint) {
	for _, v := range stops {
		g.nodes = append(g.nodes, newNode(route, v))
	}
}

func (g *graph) print() {
	for i, v := range g.nodes {
		fmt.Printf("%-2v", *v)
		var j uint = g.matrix.size * uint(i)
		for ; j < g.matrix.size*uint(i+1); j++ {
			fmt.Printf("%3v", g.matrix.table[j])
		}
		fmt.Println()
	}
}

func (g *graph) connectNodes() {
	g.matrix = newAdjMatrix(uint(len(g.nodes)))
	var i uint = 0
	for ; i < uint(len(g.nodes)); i++ {
		g.nodes[i].index = i
		var j uint = 0
		for ; j < g.matrix.size; j++ {
			if i == j {
				// *g.matrix.at(i, j) = -1
				if j > 0 && g.nodes[i-1].route == g.nodes[i].route {
					*g.matrix.at(i, j-1) = 7
				}
				if j < g.matrix.size-1 && g.nodes[i+1].route == g.nodes[i].route {
					*g.matrix.at(i, j+1) = 7
				}
			} else if g.nodes[i].label == g.nodes[j].label {
				*g.matrix.at(i, j) = 12
			}
		}
	}
}

func (g *graph) reset() {
	for i, _ := range g.nodes {
		g.nodes[i].dist = max_dist
		g.nodes[i].visited = false
	}
}

func (g *graph) findStartNodes(start uint) []*node {
	result := make([]*node, 0)
	for _, v := range g.nodes {
		if v.label == start {
			result = append(result, v)
		}
	}
	return result
}

func (g *graph) findMinNode() *node {
	var retNode *node = nil

	for _, v := range g.nodes {
		if v.label != max_dist && !v.visited {
			retNode = v
			break
		}
	}

	if retNode == nil {
		return retNode
	}

	var i uint = retNode.index
	for ; i < uint(len(g.nodes)); i++ {
		if g.nodes[i].dist < retNode.dist && !g.nodes[i].visited {
			retNode = g.nodes[i]
		}
	}

	return retNode
}

func (g *graph) findAndRelaxSiblings(n *node) {
	// var i uint = 0
	var j uint = g.matrix.size * n.index
	for ; j < g.matrix.size*(n.index+1); j++ {
		if g.matrix.table[j] != 0 && uint(g.matrix.table[j])+n.dist < g.nodes[j-g.matrix.size*n.index].dist {
			g.nodes[j-g.matrix.size*n.index].dist = uint(g.matrix.table[j]) + n.dist
		}
	}
	n.visited = true
}

func (g *graph) fillResults(stopLabel uint, results *[]uint) {
	for _, v := range g.nodes {
		if v.label == stopLabel {
			*results = append(*results, v.dist)
		}
	}
}

func (g *graph) findShortestPath(start, stop uint, stringNum int, c chan<- resultStruct) {
	results := make([]uint, 0)
	startNodes := g.findStartNodes(start)

	if len(startNodes) == 0 {
		//fmt.Println("None")
		c <- resultStruct{
			index:  stringNum,
			result: 0}

		return
	}
	for _, node := range startNodes {
		g.reset()
		node.dist = 0
		for {
			minNode := g.findMinNode()
			// fmt.Println("min node: ", minNode)
			if minNode == nil {
				break
			}
			g.findAndRelaxSiblings(minNode)
		}

		g.fillResults(stop, &results)
		// g.print()
	}

	if len(results) == 0 {
		fmt.Println("None")
		return
	} else {
		var minDist uint = results[0]
		for _, v := range results[1:] {
			if v < minDist {
				minDist = v
			}
		}
		if minDist == max_dist {
			//fmt.Println("None")
			c <- resultStruct{
				index:  stringNum,
				result: 0}
		} else {
			c <- resultStruct{
				index:  stringNum,
				result: minDist}
		}
	}
}

func process(s string, stringNum int, c chan<- resultStruct) {
	splts := strings.Split(s, " ")

	points_match := points_re.FindStringSubmatch(splts[0])
	if len(points_match) != 3 {
		return
	}

	start, err := strconv.ParseUint(points_match[1], 10, 32)
	if err != nil {
		fmt.Errorf("Parse start node failed\n")
		return
	}

	stop, err := strconv.ParseUint(points_match[2], 10, 32)
	if err != nil {
		fmt.Errorf("Parse end node failed\n")
		return
	}

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
	g.connectNodes()
	g.findShortestPath(uint(start), uint(stop), stringNum, c)
}

func main() {
	t0 := time.Now()
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

	stringNum := 0
	resultChan := make(chan resultStruct)

	for sc.Scan() {
		go process(sc.Text(), stringNum, resultChan)
		stringNum++
	}
	results := make([]uint, stringNum)

	for i := 0; i < stringNum; i++ {
		r := <-resultChan
		results[r.index] = r.result
	}

	for _, v := range results {
		if v == 0 {
			fmt.Println("None")
		} else {
			fmt.Println(v)
		}
	}

	t1 := time.Now()
	fmt.Printf("Elapsed: %v\n", t1.Sub(t0))
}
