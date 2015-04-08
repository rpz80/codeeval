package main

import(
    "fmt"
    "bufio"
    "os"
    "strings"
    "time"
    "regexp"
)

func buildre(s string) string {
    result := "^"
    for _, v := range s {
        if v == '0' {
            result += "A+"
        } else if v == '1' {
            result += "(?:B+|A+)"
        } else {
            panic("aa")
        }
    }
    return result+"$"
}

func process(s string) {
    if len(s) == 0 {
        return
    } else {
        splts := strings.Split(s, " ")
        left, right := buildre(splts[0]), splts[1]
        mask_re := regexp.MustCompile(left)
        if mask_re.MatchString(right) {
            fmt.Println("Yes")
        } else {
            fmt.Println("No")
        }
    }
}

func main() {
    start := time.Now()
    if len(os.Args) != 2 {
        fmt.Println("Usage: st <file-name>")
        return
    } else {
        if f, err := os.Open(os.Args[1]); err != nil {
            fmt.Println("File open error", err)
            return
        } else {
            defer f.Close()
            sc := bufio.NewScanner(f)
            for sc.Scan() {
                process(sc.Text())
            }
        }
    }
    end := time.Now()
    fmt.Println("Elapsed: ", end.Sub(start))
}