import sys

def process(s):
    n, ab = int(s.strip().split(',')[0]), s.strip().split(',')[1]
    result = []
    for_(ab, "", n, result)
    print(','.join(sorted(result)))

def for_(ab, s, n, result):
    if n == 0:
        if s not in result:
            result.append(s)
    else:
        for c in ab:
            for_(ab, s+c, n-1, result)

with open(sys.argv[1]) as f:
    for line in f:
        process(line)