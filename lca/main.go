package main

import(
	"fmt"
	"strings"
	"bufio"
	"os"
  "strconv"
  // "sort"
	)

type Tree struct {
	val int
	left *Tree
	right *Tree
}

func (t *Tree) addChild(l *Tree, side int) *Tree {
	if t == nil {
		panic("Tree is nil")
	}
	switch side {
		case 0:
			t.left = l
		case 1:
			t.right = l
	}
	return l
}

func MakeTree(c chan *Tree) {
	root := &Tree{val: 30}
	root.addChild(&Tree{val: 8, left: &Tree{val: 3}, right: &Tree{val: 20, left: &Tree{val: 10}, right: &Tree{val: 29}}}, 0)
	root.addChild(&Tree{val: 52}, 1)
	c <- root
}

type TraRes struct {
  val int
  done bool
}

func TraverseTo(t *Tree, val int, found *bool, c chan *TraRes) {
  if t == nil {
    return
  }
  if t.val == val {
    *found = true
    if val == 30 {
      c <- &TraRes{val,true}
    } else {
      c <- &TraRes{val,false}
    }
    return
  }
  if !*found {
    TraverseTo(t.left,val,found,c)
  }
  if !*found {
    TraverseTo(t.right,val,found,c)
  }
  if *found {
    if t.val == 30 {
      c <- &TraRes{t.val,true}
    } else {
      c <- &TraRes{t.val,false}
    }
  }
}

func process(s string, t *Tree) {
  if len(s) == 0 {
    return
  }
  nstrs := strings.Split(s, " ")
  nums := make([]int, 2)
  for i:= 0; i < 2; i++ {
    p, err := strconv.ParseInt(nstrs[i], 10, 32)
    if err != nil {
      fmt.Println(os.Stderr, err)
    }
    nums[i] = int(p)
  }
  left := make([]int,0,10)
  right := make([]int,0,10)
  
  left_flag := false
  right_flag := false
  
  left_done_flag := false
  right_done_flag := false  

  right_chan := make(chan *TraRes)
  left_chan := make(chan *TraRes)  

  go TraverseTo(t, nums[0], &left_flag, left_chan)
  go TraverseTo(t, nums[1], &right_flag, right_chan)
  for {
    if !left_done_flag {
      left_res := <- left_chan
      left_done_flag = left_res.done
      left = append(left, left_res.val)
    }
    if !right_done_flag {
      right_res := <- right_chan
      right_done_flag = right_res.done
      right = append(right, right_res.val)
    }
    if left_done_flag && right_done_flag {
      break
    }
  }

    // fmt.Println(left)
    // fmt.Println(right)

  for i, j := len(left) - 1, len(right) - 1; i >=0 && j >=0; i, j = i - 1, j -1 {
    // fmt.Println(left[i], right[j])
    if left[i] != right[j] {
      fmt.Println(left[i+1])
      break
    }
    if i == 0 {
      fmt.Println(left[i])
      break
    } else if j == 0 {
      fmt.Println(right[j])
      break
    }
  }
}

func main() {
	f, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Println(os.Stderr, "Error opening file:", err)
	}
  c := make(chan (*Tree))
  go MakeTree(c)
	scanner := bufio.NewScanner(f)
  root := <- c
	for scanner.Scan() {
		process(scanner.Text(), root)
	}
}