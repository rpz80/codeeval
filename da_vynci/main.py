import sys


def calc_substr(s1, s2):
    while True:
        i = s1.find(s2[0])
        if i == -1:
            break
        if i == len(s1) - 1:
            return 1
        i += 1
        j = 1
        while j < len(s2):
            if i == len(s1) - 1:
                return j
            if s1[i] != s2[j]:
                s1 = s1[i+1:]
                break
            i += 1
            j += 1
            if j == len(s2):
                return len(s2)-1
    return 0


def find_max_overlap(l):
    max_overlap = 0
    max1 = 0
    max2 = 0
    for i in range(0, len(l) - 1):
        for j in range(i+1, len(l)):
            tmp = calc_substr(l[i], l[j])
            if tmp > max_overlap:
                max1 = i
                max2 = j
                max_overlap = tmp
            tmp = calc_substr(l[j], l[i])
            if tmp > max_overlap:
                max2 = i
                max1 = j
                max_overlap = tmp
    return max1, max2, max_overlap


def process(s):
    splts = s.strip().split(';')
    while len(splts) != 1:
        m1, m2, mo = find_max_overlap(splts)
        result_s = splts[m1][:-mo-1] + splts[m2]
        splts[m1] = result_s
        del(splts[m2])
    print(splts[0])


def main():
    with open(sys.argv[1]) as f:
        for line in f:
            process(line)


if __name__ == '__main__':
    main()