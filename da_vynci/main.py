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
            if s1[i] != s2[j]:
                s1 = s1[i:]
                break
            if i == len(s1) - 1:
                return j+1, outer
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
            if tmp > max_overlap and ol != no:
                max1 = i
                max2 = j
                max_overlap, ol_type = tmp, ol
            tmp, ol = calc_substr(l[j], l[i])
            if tmp > max_overlap and ol != no:
                max2 = i
                max1 = j
                max_overlap, ol_type = tmp, ol
    return max1, max2, max_overlap, ol_type


def process(s):
    splts = s.strip().split(';')
    while len(splts) != 1:
        m1, m2, mo, ol_type = find_max_overlap(splts)
        # print('merging {}  <<<<>>>>  {}'.format(splts[m1], splts[m2]))
        if ol_type == inner:
            # print('ol_type: {} result: {}'.format(ol_type, splts[m1]))
            del(splts[m2])
        elif ol_type == outer:
            splts[m1] = splts[m1][:-mo] + splts[m2]
            # print('ol_type: {} result: {}'.format(ol_type, splts[m1]))
            del(splts[m2])
    return splts[0]


def main():
    with open(sys.argv[1], 'r') as f:
        for line in f:
            print(process(line))


def merge(s1, s2, mo, olt):
    if olt == inner:
        return s1
    elif olt == outer:
        return s1[:-mo] + s2
    else:
        raise Exception('unknown overlap type')


def test_merge(s1, s2, check_string):
    mo1, ol1 = calc_substr(s1, s2)
    mo2, ol2 = calc_substr(s2, s1)
    if mo1 >= mo2:
        print('{:10} == {:10}: {}'.format(merge(s1, s2, mo1, ol1),
                                          check_string,
                                          merge(s1, s2, mo1, ol1) == check_string))
    elif mo2 > mo1:
        print('{:10} == {:10}: {}'.format(merge(s2, s1, mo2, ol2),
                                          check_string,
                                          merge(s2, s1, mo2, ol2) == check_string))


def tests():
    test_merge('ab', 'abcd', 'abcd')
    test_merge('abcd', 'ab', 'abcd')
    test_merge('fcabcd', 'ab', 'fcabcd')
    test_merge('abcd', 'cdui', 'abcdui')
    test_merge('abcd', 'nkiuab', 'nkiuabcd')
    test_merge('abcd', 'd', 'abcd')
    test_merge('ab', 'bd', 'abd')
    test_merge('abcd', 'fabc', 'fabcd')
    test_merge('abcdv', 'cd', 'abcdv')
    test_merge(', wh*ich w#as written on pap',
               'written on papyrus. )The. box co.',
               ', wh*ich w#as written on papyrus. )The. box co.')
    test_merge('abcd', 'abcd', 'abcd')
    try:
        test_merge('abcd', 'nkiuabu', '')
    except:
        print("test_merge('abcd', 'nkiuabu', '') raised an exception as expected")
    try:
        test_merge('ABCDEF', 'XCDEZ', '')
    except:
        print("test_merge('ABCDEF', 'XCDEZ', '') raised an exception as expected")

    print(process('O draconia;conian devil! Oh la;h lame sa;saint!')=='O draconian devil! Oh lame saint!')
    print(process('m quaerat voluptatem.;pora incidunt ut labore et d;, consectetur, adipisci velit;olore magnam aliqua;idunt ut labore et dolore magn;uptatem.;i dolorem ipsum qu;iquam quaerat vol;psum quia dolor sit amet, consectetur, a;ia dolor sit amet, conse;squam est, qui do;Neque porro quisquam est, qu;aerat voluptatem.;m eius modi tem;Neque porro qui;, sed quia non numquam ei;lorem ipsum quia dolor sit amet;ctetur, adipisci velit, sed quia non numq;unt ut labore et dolore magnam aliquam qu;dipisci velit, sed quia non numqua;us modi tempora incid;Neque porro quisquam est, qui dolorem i;uam eius modi tem;pora inc;am al')=='Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem.')


if __name__ == '__main__':
    # tests()
    main()