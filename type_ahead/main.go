package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"sort"
	"strconv"
	"strings"
	"unicode"
)

const text = `Mary had a little lamb its fleece was white as snow;
And everywhere that Mary went, the lamb was sure to go.
It followed her to school one day, which was against the rule;
It made the children laugh and play, to see a lamb at school.
And so the teacher turned it out, but still it lingered near,
And waited patiently about till Mary did appear.
"Why does the lamb love Mary so?" the eager children cry; "Why, Mary loves the lamb, you know" the teacher did reply."`

func statNgramm(a int, ng []map[string]int) []map[string]int {
	splts := regexp.MustCompile(`\s+|(?:\r\n)+|\n+|\r+`).Split(text, -1)
	trimF := func(r rune) bool {
		return !unicode.IsLetter(r)
	}
	for i := range splts {
		splts[i] = strings.TrimFunc(splts[i], trimF)
		// splts[i] = strings.ToLower(splts[i])
	}
	ngDict := make(map[string]int)
	for i := 0; i < len(splts)-a+1; i++ {
		key := splts[i] + " "
		for j := i + 1; j < i+a; j++ {
			key += splts[j]
			if j != i+a-1 {
				key += " "
			}
		}
		ngDict[key] += 1
	}
	ng = append(ng, ngDict)
	return ng
}

func process(s string, ngrams []map[string]int) {
	splts := strings.Split(s, ",")
	if n, err := strconv.Atoi(splts[0]); err != nil {
		fmt.Println(err)
		return
	} else {
		// splts[1] = strings.ToLower(splts[1])
		findMatched(ngrams[n-2], splts[1])
	}
}

type result struct {
	s     string
	count int
}
type By []result

func (a By) Len() int      { return len(a) }
func (a By) Swap(i, j int) { a[i], a[j] = a[j], a[i] }
func (a By) Less(i, j int) bool {
	if a[i].count != a[j].count {
		return a[i].count > a[j].count
	} else {
		return a[i].s < a[j].s
	}
}

func findMatched(ng map[string]int, s string) {
	results := make([]result, 0, 16)
	for k, v := range ng {
		if strings.Index(k, s) == 0 {
			results = append(results, result{k, v})
		}
	}
	total := 0
	for i := range results {
		total += results[i].count
	}
	resultString := ""
	sort.Sort(By(results))
	for i := range results {
		splts := strings.Split(results[i].s, " ")
		prc := float64(results[i].count) / float64(total)
		// fmt.Println(splts[len(splts)-1 : len(splts)][0], results[i].count, float64(total))
		resultString += splts[len(splts)-1 : len(splts)][0] + "," + fmt.Sprintf("%.3f", prc)
		if i != len(results)-1 {
			resultString += ";"
		}
	}
	fmt.Println(resultString)
}

func main() {
	defer func() {
		if err := recover(); err != nil {
			fmt.Fprintf(os.Stderr, "Fatal: %v\n", err)
		}
	}()
	if len(os.Args) < 2 {
		fmt.Fprintf(os.Stdout, "Usage: build_ngramm <fname>")
		return
	}
	if f, err := os.Open(os.Args[len(os.Args)-1]); err != nil {
		fmt.Fprintf(os.Stderr, "File open error")
		return
	} else {
    defer f.Close()
		ngrams := make([]map[string]int, 0, 3)
		for i := 2; i < 5; i++ {
			ngrams = statNgramm(i, ngrams)
		}
		sc := bufio.NewScanner(f)
		for sc.Scan() {
			process(sc.Text(), ngrams)
		}
	}
}
