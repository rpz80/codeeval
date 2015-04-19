import sys
import re


item_re = re.compile(r'\((.+),(.+),(.+)\)')
i_d, weight, price = 0, 1, 2


def process(items, max_sum):
    results = []
    for i in range(0, len(items)):
        foo(items, [], max_sum, i, results)
    if len(results) == 0:
        print('-')
    else:
        ret = results[0]
        ret_sum = sum_by_price(ret)
        for i in range(1, len(results)):
            tmp = sum_by_price(results[i])
            if tmp > ret_sum or (tmp == ret_sum and sum_by_weight(results[i]) < sum_by_weight(ret)):
                ret_sum = tmp
                ret = results[i]
        print(','.join([str(x[i_d]) for x in ret]))


def sum_by_price(l):
    return sum([x[price] for x in l])


def sum_by_weight(l):
    return sum([x[weight] for x in l])


def foo(l, start, max_sum, index, results):
    if l[index][weight] + sum_by_weight(start) <= max_sum:
        start.append(l[index])
    else:
        return
    index += 1
    found = False
    while index < len(l):
        if l[index][weight] + sum_by_weight(start) <= max_sum:
            found = True
            foo(l, start[:], max_sum, index, results)
        index += 1
    if not found:
        results.append(start)


def main():
    with open(sys.argv[1]) as f:
        for line in f:
            splts = line.strip().split(':')
            max_sum = int(splts[0].strip())
            splts = splts[1].strip().split(' ')
            items = []
            for s in splts:
                items.append([int(item_re.match(s).group(1)),
                              float(item_re.match(s).group(2)),
                              int(item_re.match(s).group(3)[1:])])
            # print(items)
            process(items, max_sum)


if __name__ == '__main__':
    main()