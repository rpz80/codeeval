import sys
test_cases = open(sys.argv[1], 'r')
for test in test_cases:
    l = test.strip().split(' ')
    l = l[1:]
    l = [ i for i in map(int, l) ]
    m = 500000
    
    for i in range(1, max(l)):
        tmp = 0
        for j in l:
            tmp += abs(i-j)
        if tmp < m:
            m = tmp
    
    print(m)
    
test_cases.close()