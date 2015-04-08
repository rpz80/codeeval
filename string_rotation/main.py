import sys

if __name__ == '__main__':
  with open(sys.argv[1]) as f:
    for line in f:
      left, right = line.split(',')
      right = right.strip()
      if left in right*2 and len(left) == len(right):
        print(True)
      else:
        print(False)