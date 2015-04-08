package main

import(
    "fmt"
    "bufio"
    "os"
    "strings"
    )

func splitWTrim(s string) ([]string) {
    scanner := bufio.NewScanner(strings.NewReader(s))
    scanner.Split(customSplit)
    res := make([]string, 0, 32)
    for scanner.Scan() {
        res = append(res, scanner.Text())
    }
    return res
}

func process(s string) {
    strs := strings.Split(s, ";")
    lefts, rights := splitWTrim(strs[0]), splitWTrim(strs[1])
    res := ""
    j := 0
    for i := 0; i < len(lefts); i++ {
        if(j < len(rights)){
            if ind := strings.Index(lefts[i], rights[j]); ind != -1 {
                res += strings.Repeat("_", ind)
                res += rights[j]
                res += strings.Repeat("_", len(lefts[i]) - ind - len(rights[j]))
                res += " "
                j++
            } else {
                res += strings.Repeat("_", len(lefts[i]))
                res += " "
            }
        } else {
            res += strings.Repeat("_", len(lefts[i]))
            res += " "
        }
    }
    if(j < len(rights)) {
        fmt.Println("I cannot fix history")
    } else {
        fmt.Println(res)
    }
}

func customSplit(data []byte, atEOF bool) (advance int, token []byte, err error) {
    advance, token, err = bufio.ScanWords(data, atEOF)
    return
}

func main() {
    defer func() {
        r := recover()
        if r != nil {
            fmt.Fprintln(os.Stderr, "Fatal: ", r)
            return
        }        
    }()
    f, err := os.Open(os.Args[1])
    if err != nil {
        fmt.Fprintln(os.Stderr, "File open error: ", err)
    }
    defer f.Close()
    scanner := bufio.NewScanner(f)
    for scanner.Scan() {
        process(scanner.Text())
    }
}
