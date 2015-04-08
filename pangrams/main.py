import sys

if __name__ == '__main__':
  alphabet = {chr(x) for x in range(97, 123)}
  with open(sys.argv[1]) as f:
    for line in f:
      alphaline = {x.lower() for x in line if x.isalpha()}
      result = ''.join(sorted(alphabet.difference(alphaline)))
      if not result:
        print('NULL')
      else:
        print(result)
     