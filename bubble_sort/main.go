package main

import(
    "bufio"
    "os"
    "fmt"
    "strconv"
    "strings"
    "time"
    )

type Ar []int

func (a Ar) String() string {
    result := ""
    for i := 0; i < len(a); i++{
        s := fmt.Sprintf("%v", a[i])
        result = result + s
        if i != len(a) - 1 {
            result += " "
        }
    }
    return result
}

func bubble_sort(a []int, count int) {
    if count == 0 {
        return
    }
    for i := 0; i < len(a)-1; i++ {
        found := false
        for j := 0; j < len(a) - 1 - i; j++ {
            if a[j] > a[j+1] {
                a[j], a[j+1] = a[j+1], a[j]
                if !found {
                    count--
                    found = true
                }
            }
        }
        if count == 0 {
            return
        }
    }
}

func process(s string) {
    splitted := strings.Split(s, "|")
    s, sort_count := splitted[0], strings.Trim(splitted[1], " ")

    scanner := bufio.NewScanner(strings.NewReader(s))
    scanner.Split(bufio.ScanWords)

    a := make([]int, 0 , 64)
    for scanner.Scan() {
        n, err := strconv.ParseInt(scanner.Text(), 10, 32)
        if err != nil {
            fmt.Fprintf(os.Stderr, "Conv err: ", err)
            continue
        }
        a = append(a, int(n))
    }
    sc, err := strconv.ParseInt(sort_count, 10, 32)
    if err != nil {
        fmt.Fprintf(os.Stderr, "Conv err: ", err)
    }
    bubble_sort(a, int(sc))
    fmt.Println(Ar(a))
}

func main() {
    start := time.Now()
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
    end := time.Now()
    fmt.Println("Elapsed: ", end.Sub(start))
}