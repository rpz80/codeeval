import sys

def is_pali(n):
  pali, oldn = n%10, n
  n = int(n/10)
  while n != 0:
    pali *= 10
    pali += n%10
    n = int(n/10)
  return pali == oldn

def process(s): 
  l, r = map(int, s.strip().split(' '))
  count = 0
  for i in range(l, r+1):
    for j in range(i, r+1):
      range_count = 0
      for k in range(i, j+1):
        if is_pali(k):
          range_count += 1
      if range_count%2 == 0:
        count += 1
  print(count)
  
def main():
  with open(sys.argv[1]) as f:
    for line in f:
      process(line)

if __name__ == '__main__':
  main()
