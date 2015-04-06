import sys

delta_2 = [-2, 2]
delta_1 = [-1, 1]

def valid_spot(x, y):
    if x < 1 or x > 8 or y < 1 or y > 8: 
        return False
    else: 
        return True

def next_spot(x, y):
    for dx in delta_1:
        for dy in delta_2:
            yield (x + dx, y + dy)
    for dy in delta_1:
        for dx in delta_2:
            yield (x + dx, y + dy)

def find_all_valid(coord):
    y = int(coord[1])
    x = ord(coord[0]) - 96
    result = []   
    for move in next_spot(x, y):
        if valid_spot(*move):
            result.append(chr(move[0] + 96) + str(move[1]))
    print(' '.join(sorted(result)))

def main():
    with open(sys.argv[1]) as f:
        for line in f:
            find_all_valid(line)

main()
