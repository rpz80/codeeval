import sys


inner, outer, no = 0, 1, 2


def calc_substr(s1, s2):
    while True:
        i = s1.find(s2[0])
        if i == -1:
            break
        if i == len(s1) - 1:
            return 1, outer
        i += 1
        j = 1
        while True:
            if j == len(s2) - 1:
                return len(s2), inner
            if i == len(s1) - 1:
                return j+1, outer
            if s1[i] != s2[j]:
                s1 = s1[i+1:]
                break
            i += 1
            j += 1
    return 0, no


def find_max_overlap(l):
    max_overlap = 0
    max1 = 0
    max2 = 0
    ol_type = no
    for i in range(0, len(l) - 1):
        for j in range(i+1, len(l)):
            tmp, ol = calc_substr(l[i], l[j])
            if tmp > max_overlap:
                max1 = i
                max2 = j
                max_overlap, ol_type = tmp, ol
            tmp, ol = calc_substr(l[j], l[i])
            if tmp > max_overlap:
                max2 = i
                max1 = j
                max_overlap, ol_type = tmp, ol
    return max1, max2, max_overlap, ol_type


def process(s):
    splts = s.strip().split(';')
    while len(splts) != 1:
        m1, m2, mo, ol_type = find_max_overlap(splts)
        if ol_type == inner:
            del(splts[m2])
        elif ol_type == outer:
            splts[m1] = splts[m1][:-mo] + splts[m2]
            del(splts[m2])
    return splts[0]


def main():
    with open(sys.argv[1]) as f:
        for line in f:
            print(process(line))


def tests():
    s1 = 'abcd'
    s2 = 'ab'
    print(calc_substr(s1, s2))
    print(calc_substr(s2, s1))

    ol, type = calc_substr(s1, s2)
    if type == inner:
        print('new string: ', s1)

    ol, type = calc_substr(s2, s1)
    if type == outer:
        print('new string: ', s2[:-ol] + s1)

    print(process('O draconia;conian devil! Oh la;h lame sa;saint!'))
    print(process('m quaerat voluptatem.;pora incidunt ut labore et d;, consectetur, adipisci velit;olore magnam aliqua;idunt ut labore et dolore magn;uptatem.;i dolorem ipsum qu;iquam quaerat vol;psum quia dolor sit amet, consectetur, a;ia dolor sit amet, conse;squam est, qui do;Neque porro quisquam est, qu;aerat voluptatem.;m eius modi tem;Neque porro qui;, sed quia non numquam ei;lorem ipsum quia dolor sit amet;ctetur, adipisci velit, sed quia non numq;unt ut labore et dolore magnam aliquam qu;dipisci velit, sed quia non numqua;us modi tempora incid;Neque porro quisquam est, qui dolorem i;uam eius modi tem;pora inc;am al')=='Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem.')


if __name__ == '__main__':
    #tests()
    main()