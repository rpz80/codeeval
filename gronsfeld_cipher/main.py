import sys

alphabet = ' !"#$%&\'()*+,-./0123456789:<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'

def decrypt(key, mes):
    i = 0
    result = []
    for c in mes:
        result.append(alphabet[alphabet.find(c) - int(key[i])])
        i += 1
        if i == len(key):
            i = 0
    return ''.join(result)

def main():
    with open(sys.argv[1]) as f:
        for line in f:
            line = line.strip('\n').split(';')
            key, mes = line[0], line[1]
            print(''.join(decrypt(key, mes)))

main()