package main

import(
    "fmt"
    "os"
    "bufio"
)

const(
    ROW = 3
    COL = 4
    )

func NewSearchTable() [][]byte {
    ret := make([][]byte, ROW)
    for i := range ret {
        ret[i] = make([]byte, COL)
    }
    ret[0][0] = 'A'; ret[0][1] = 'B'; ret[0][2] = 'C'; ret[0][3] = 'E'
    ret[1][0] = 'S'; ret[1][1] = 'F'; ret[1][2] = 'C'; ret[1][3] = 'S'
    ret[2][0] = 'A'; ret[2][1] = 'D'; ret[2][2] = 'E'; ret[2][3] = 'E'

    return ret
}

func NewVisitorTable() [][]byte {
    ret := make([][]byte, ROW)
    for i := range ret {
        ret[i] = make([]byte, COL)
    }
    for i := range ret {
        for j := 0; j < COL; j++ {
            ret[i][j] = 0
        }
    }
    return ret
}

type Point struct {
    x,y int
}

func FindFirstLetters(st, vt [][]byte, letter byte) []*Point {
    result := make([]*Point,0,2)
 
    for i:= range vt {
        for j := 0; j < COL; j++ {
            if st[i][j] == letter && vt[i][j] == 0 {
                result = append(result, &Point{i, j}) 
            }
        }
    }
    return result
}

func FindNextLetter(st, vt [][]byte, letter byte, i, j int) (int, int) {
    switch {
        case i-1 >= 0  && vt[i-1][j] == 0 && st[i-1][j] == letter: return i-1, j
        case i+1 < ROW && vt[i+1][j] == 0 && st[i+1][j] == letter: return i+1, j
        case j-1 >= 0  && vt[i][j-1] == 0 && st[i][j-1] == letter: return i, j-1
        case j+1 < COL && vt[i][j+1] == 0 && st[i][j+1] == letter: return i, j+1
    }
    return -1,-1
}

func TrySearch(st, vt [][]byte, pattern string, i, j int, result []byte) bool {    

    if len(result) == len(pattern) {
        return true
    } else {
        x, y := FindNextLetter(st, vt, pattern[len(result)], i, j)
        if x == -1 {
            return false
        } else {
            retval := false
            if i == 1 && j == 3 {
                
                vt2 := NewVisitorTable()
                result2 := make([]byte, len(result))
                copy(result2, result)

                for ind:= range vt2{
                    for jind := 0; jind < COL; jind++ {
                        vt2[ind][jind] = vt[ind][jind]
                    }
                }
                if x == 0 && y == 3 && vt[2][3] == 0 {
                    vt[x][y] = 1
                    vt2[2][3] = 1

                    result = append(result, st[x][y])
                    result2 = append(result2, st[2][3])

                    retval = retval || TrySearch(st, vt, pattern, x, y, result)
                    retval = retval || TrySearch(st, vt2, pattern, 2, 3, result2)
                    return retval
                } else if x == 2 && y == 3 && vt[0][3] == 0 {
                    vt[x][y] = 1
                    vt2[0][3] = 1

                    result = append(result, st[x][y])
                    result2 = append(result2, st[0][3])

                    retval = retval || TrySearch(st, vt, pattern, x, y, result)
                    retval = retval || TrySearch(st, vt2, pattern, 0, 3, result2)
                    return retval
                } else {
                    vt[x][y] = 1
                    result = append(result, st[x][y])
                    return TrySearch(st, vt, pattern, x, y, result)
                }

            } else if i == 1 && j == 0 {

                vt2 := NewVisitorTable()
                result2 := make([]byte, len(result))
                copy(result2, result)

                for ind:= range vt2{
                    for jind := 0; jind < COL; jind++ {
                        vt2[ind][jind] = vt[ind][jind]
                    }
                }

                if x == 0 && y == 0 && vt[2][0] == 0 {
                    vt[x][y] = 1
                    vt2[2][0] = 1

                    result = append(result, st[x][y])
                    result2 = append(result2, st[2][0])

                    retval = retval || TrySearch(st, vt, pattern, x, y, result)
                    retval = retval || TrySearch(st, vt2, pattern, 2, 0, result2)
                    return retval
                } else if x == 2 && y == 0 && vt[0][0] == 0 {
                    vt[x][y] = 1
                    vt2[0][0] = 1

                    result = append(result, st[x][y])
                    result2 = append(result2, st[0][0])

                    retval = retval || TrySearch(st, vt, pattern, x, y, result)
                    retval = retval || TrySearch(st, vt2, pattern, 0, 0, result2)
                    return retval
                } else {
                    vt[x][y] = 1
                    result = append(result, st[x][y])
                    return TrySearch(st, vt, pattern, x, y, result)
                }
            } else {
                vt[x][y] = 1
                result = append(result, st[x][y])
                return TrySearch(st, vt, pattern, x, y, result)
            }
        }        
    }
    return false
}

func process(s string) {
    if len(s) == 0 {
        return
    }
    st, vt := NewSearchTable(), NewVisitorTable()
    points := FindFirstLetters(st, vt, s[0])

    retval := false

    for _, v := range points {
        result := make([]byte, 0, 32)
        result = append(result, st[v.x][v.y])

        vt = NewVisitorTable()
        vt[v.x][v.y] = 1
        retval = TrySearch(st, vt, s, v.x, v.y, result)
        if retval {
            break
        }
    }

    if retval {
        fmt.Println("True")
    } else {
        fmt.Println("False")
    }
}

func main() {
    defer func() {
        if r := recover(); r != nil {
            fmt.Fprintln(os.Stderr, "Fatal error: ", r)
            return
        }
    }()
    f, err := os.Open(os.Args[1])

    if err != nil {
        fmt.Fprintln(os.Stderr, "File open error: ", err)
        return
    }
    scanner := bufio.NewScanner(f)

    for scanner.Scan() {
        process(scanner.Text())
    }
}