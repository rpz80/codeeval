package main

import(
    "fmt"
    "os"
    "bufio"
    //"time"
    // "strconv"
    )

const(
    A = iota
    R = iota
    )

var rdict = map[byte]int{'I':1, 'V':5, 'X':10, 'L':50, 'C':100, 'D':500, 'M':1000}

func process(s string) {
    if len(s) == 0 {
        return
    }

    result, cur_ar, last_ar := 0, 0, 0
    last_r, state := -1, A

    for _, c := range s {
        switch state {
        case A:
            last_ar = cur_ar
            cur_ar = int(c) - 48
            state = R
        case R:
            if last_r != -1 && rdict[byte(c)] > last_r {
                result -= 2*last_r*last_ar
            }
            result += cur_ar*rdict[byte(c)]
            state = A
            last_r = rdict[byte(c)]
        }
    }
    fmt.Println(result)
}

func main() {
    //start := time.Now()
    defer func() {
        if err := recover(); err != nil {
            fmt.Fprintf(os.Stderr, "Fatal: ", err)
        }
    }()

    f, err := os.Open(os.Args[1])
    if err != nil {
        fmt.Fprintf(os.Stderr, "File open error: ", err)
        return
    }
    defer f.Close()
    sc := bufio.NewScanner(f)

    for sc.Scan() {
        process(sc.Text())
    }
    
    //end := time.Now()
    //fmt.Println("Elapsed: ", end.Sub(start))
}