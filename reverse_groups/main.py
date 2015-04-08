import sys

if __name__ == '__main__':
  with open(sys.argv[1]) as f:
    for line in f:
      left, right = line.split(';')
      left = left.split(',')
      left = [int(x) for x in left]
      k = int(right)
      group_index = k
      result = []
      while k <= len(left):
        result += sorted(left[k-group_index:k], reverse=True)
        k += group_index
      result += left[k-group_index:]  
      result = [str(x) for x in result]
      print(','.join(result))