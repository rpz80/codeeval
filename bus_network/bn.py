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


def relax_start_node(start, g):
    for i in range(1, len(g)):
        if g[i][0][2] == start:
            g[i][0][0] = 0
            # row[0][3] = True


def find_min_node(g):
    min_node, dist = 1, g[1][0][0]
    for i in range(2, len(g)):
        if g[i][0][0] < g[min_node][0][0]:
            min_node, dist = i, g[i][0][0]
    return min_node, dist


def find_node(n, g):
    for i in range(1, len(g)):
        if n[1] == g[i][0][1] and n[2] == g[i][0][2]:
            return i, g[i][0][0]


def find_siblings(n, g):
    result = []
    for j in range(1, len(g)):
        if j == n[0]:
            for s in g[j][1:]:
                if s != -1 and s != 1:
                    ind, dist = find_node(g[0][j], g)
                    if not g[ind][0][3]:
                        result.append((ind, dist + s))
    return result


def relax(siblings, g):
    for s in siblings:
        if g[s[0]][0][1] > s[1]:
            g[s[0]][0][1] = s[1]


def find_stop(stop, siblings):
    for s in siblings:
        if s[0] == stop:
            return s
    return None


def distance(stop_node, g):
    for i in range(1, len(g)):
        if i == stop_node[0]:
            return g[i][0][0]


def mark_visited(n, g):
    for i in range(1, len(g)):
        if i == n[0]:
            g[i][0][3] = True


def find_shortest_path(g, start, stop):
    results = []
    relax_start_node(start, g)
    while 1:
        n = find_min_node(g)
        siblings = find_siblings(n, g)
        if len(siblings) == 0:
            return results
        relax(siblings, g)
        mark_visited(n, g)
        stop_node = find_stop(stop, siblings)
        if stop_node is not None:
            results.append(distance(stop_node, g))


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
            start, stop = int(points_match.group(1)), int(points_match.group(2))

            graph = [['NOOP']]
            index = 1

            for b in splts[1:]:
                stops_match = stops_re.match(b)
                stops = [i for i in map(int, stops_match.group(1).split(','))]
                add_to_graph(graph, stops, index)
                index += 1

            fill_graph(graph)
            print(find_shortest_path(graph, start, stop))


if __name__ == '__main__':
    main()