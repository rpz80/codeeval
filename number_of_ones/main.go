package main

import(
    "fmt"
    "os"
    "bufio"
    "strconv"
    )

func main() {
    f, err := os.Open(os.Args[1])
    if err != nil {
        fmt.Fprintln(os.Stderr, err)
        return
    }
    defer f.Close()

    scanner := bufio.NewScanner(f)
    var counter int 
    var i uint32

    for scanner.Scan() {
        num, err := strconv.ParseInt(scanner.Text(), 10, 32)
        if err != nil {
            continue
        }

        counter = 0
        i = 0

        for ; i < 32; i++ {
            if (num >> i) & 0x1 == 1 {
                counter++
            }
        }
        fmt.Println(counter)
    }
}