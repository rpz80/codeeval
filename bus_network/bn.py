import sys
import os
import re


points_re = re.compile(r'\((\d+),\s*(\d+)\)')
stops_re = re.compile(r'R\d+=\[(.+)\]')


def add_to_graph(g, stops, ind):
    if len(g) != 0:
        for s in stops:
            g[0].append([False, ind, s])
            g.append([[False, ind, s]])
    else:
        g.append([[False, ind, s] for s in stops])
        for s in stops:
            g.append([[False, ind, s]])

    j = 1
    for row in g[1:]:
        for i in range(1, len(g[0])):
            if i == j:
                row.append(-1)
            else:



def find_shortest_path(g, start, stop):
    for row in g:
        print(row)


def main():
    if len(sys.argv) < 2:
        print("usage: {} <file>".format(os.path.basename(sys.argv[0])))
        exit(1)

    with open(sys.argv[1]) as f:
        for line in f:
            splts = line.strip().split(' ')

            points_match = points_re.match(splts[0])
            start, stop = points_match.group(1), points_match.group(2)

            graph = [['NOOP']]
            index = 1

            for b in splts[1:]:
                stops_match = stops_re.match(b)
                stops = [i for i in map(int, stops_match.group(1).split(','))]
                add_to_graph(graph, stops, index)
                index += 1

            find_shortest_path(graph, start, stop)


if __name__ == '__main__':
    main()