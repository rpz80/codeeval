package main

import(
    "fmt"
    "bufio"
    "os"
    "strings"
    "strconv"
    "sort"
    )

const(
    MAX_ANGLE = 180
    )

type Car struct {
    id float64
    vmax float64 
    taccel float64
    tdowncel float64
    lapTime float64
}

type Track []float64

type ByLT []*Car

func (a ByLT) Len() int           { return len(a) }
func (a ByLT) Swap(i, j int)      { a[i], a[j] = a[j], a[i] }
func (a ByLT) Less(i, j int) bool { return a[i].lapTime < a[j].lapTime }


func (c *Car) calcLapTime(tr Track) {
    calcAccelTime := func(aprc, fullTime float64) float64 {
        return fullTime*aprc
    }
    calcSeqTime := func(c *Car, initAngle, dist, endAngle float64) float64 {
        anglePercent := initAngle/MAX_ANGLE
        t1 := calcAccelTime(anglePercent, c.taccel)/3600
        anglePercent2 := endAngle/MAX_ANGLE
        t2 := calcAccelTime(anglePercent2, c.tdowncel)/3600

        deltav1 := c.vmax - c.vmax*(1 - anglePercent)
        deltav2 := c.vmax*(1 - anglePercent2) - c.vmax

        x1 := c.vmax*(1 - anglePercent)*t1 + deltav1*t1/2
        x2 := c.vmax*t2 + deltav2*t2/2
        t3 := (dist - x1 - x2)/c.vmax

        return t1 + t2 + t3
    }

    c.lapTime += calcSeqTime(c, 180, tr[0], tr[1])
    // fmt.Println("first seq: ", c.lapTime)
    for i := 1; i < len(tr)-2; i +=2 {
        c.lapTime += calcSeqTime(c, tr[i], tr[i+1], tr[i+2])
    }
}

func stringToFloat(s string) float64 {
    if fl, err := strconv.ParseFloat(s, 64); err == nil {
        return fl
    } else {
        fmt.Fprintln(os.Stderr, "ParseFloat error: ", err)
    }
    return -1.0
}

func NewCar(s string) *Car {
    car_string := strings.Split(s, " ")
    return &Car{stringToFloat(car_string[0]), stringToFloat(car_string[1]),
                stringToFloat(car_string[2]), stringToFloat(car_string[3]), 0.0}
}

func GetAngleSpeed(angle, top_speed float64) float64{
    return top_speed*(angle/MAX_ANGLE)
}

func process(s string, track []float64, result *[]*Car){
    car := NewCar(s)
    car.calcLapTime(track)
    *result = append(*result, car)
}

func main() {
    if len(os.Args) < 2 {
        fmt.Fprintln(os.Stderr, "Usage: cr <file-name>")
        return
    } else {
        if f, err := os.Open(os.Args[1]); err != nil {
            fmt.Fprintln(os.Stderr, "File open error")
            return
        } else {
            sc := bufio.NewScanner(f)
            sc.Scan()

            t := strings.Split(sc.Text(), " ")
            track := make([]float64, 0, 64)
            result := make([]*Car, 0, 32)

            for _, v := range t {
                if fl, err := strconv.ParseFloat(v, 64); err == nil {
                    track = append(track, fl)
                } else {
                    fmt.Fprintln(os.Stderr, "ParseFloat error: ", v, err)
                }
            }
            for sc.Scan() {
                process(sc.Text(), track, &result)
            }
            
            sort.Sort(ByLT(result))
            for _, v := range result {
                fmt.Printf("%.0f %.2f\n", v.id, v.lapTime*3600)
            }
        }
    }
}