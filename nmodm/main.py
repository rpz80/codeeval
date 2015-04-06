import sys

with open(sys.argv[1]) as f:
    for line in f:
        n, m = int(line.split(',')[0]), int(line.split(',')[1])
        if n < m:
            print(n)
        else:
            i = 1
            while True:
                if m*i == n:
                    print(0)
                    break
                elif m*i > n:
                    print(n - m*(i-1))
                    break
                i += 1
