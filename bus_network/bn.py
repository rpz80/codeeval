import sys
import os
import re


points_re = re.compile(r'\((\d+),\s*(\d+)\)')
stops_re = re.compile(r'R\d+=\[(.+)\]')


def add_to_graph(g, stops, ind):
    if len(g) != 0:
        for s in stops:
            g[0].append([100000, ind, s])
            g.append([[100000, ind, s, False]])
    else:
        g.append([[False, ind, s] for s in stops])
        for s in stops:
            g.append([[False, ind, s]])


def find_shortest_path(g, start, stop):
    results = []
    relax_start_node(start)
    while 1:
        n = find_min_node()
        siblings = find_siblings(n)
        if len(siblings) == 0:
            return results
        relax(siblings)
        stop_node = find_stop(stop)
        if stop_node is not None:
            results.append(distance(stop_node))


def fill_graph(g):
    for i in range(1, len(g)):
        for j in range(1, len(g[0])):
            if i == j:
                g[i].append(1)
            else:
                if g[i][0][1] == g[0][j][1] and abs(g[i][0][2] - g[0][j][2]) == 1:
                    g[i].append(7)
                elif g[i][0][2] == g[0][j][2]:
                        g[i].append(12)
                else:
                    g[i].append(-1)


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

            fill_graph(graph)
            find_shortest_path(graph, start, stop)


if __name__ == '__main__':
    main()