package main

import(
  "fmt"
  "strings"
  "bufio"
  "os"
  "strconv"
)
func process(s string) {
  if len(s) == 0 {
    return
  }
  number_strs := strings.Split(s, " ")
  numbers := make([]int, 2)
  for i := range numbers {
    n, err := strconv.ParseInt(number_strs[i],10,32)
    if err != nil {
      fmt.Println("Error parsing: ", err)
      return
    }
    numbers[i] = int(n)
  }

  doors := make([]bool, numbers[0])
  for i := 0; i < numbers[1]; i++ {
    if i != numbers[1] - 1 {
      ApplyLock(doors, false)
    } else {
      ApplyLock(doors, true)
    }
  }
  locked := 0
  for _, v := range doors {
    if !v {
      locked++
    }
  }
  fmt.Println(locked)
}

func ApplyLock(ar []bool, last bool) {
  if last {
    ar[len(ar)-1] = !ar[len(ar)-1]
    return
  }  
  for i := 1; i < len(ar); {
    if !ar[i] {
      ar[i] = !ar[i]
    }
    i += 2
  }

  for i := 2; i < len(ar); {
    ar[i] = !ar[i]
    i += 3
  }
}

func main() {
  defer func() {
    r := recover()
    if r != nil {
      fmt.Println("Recover: ", r)
    }
  }()
  f, err := os.Open(os.Args[1])
  if err != nil {
    fmt.Println(os.Stderr, "file open error: ", err)
    return
  }
  defer f.Close()
  scanner := bufio.NewScanner(f)
  for scanner.Scan() {
    process(scanner.Text())
  }
}