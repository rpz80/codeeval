import sys

def is_prime(n):
  i = 2
  while i*i <= n:
    if n%i == 0:
      return False
    i += 1
  return True

def process(s):
  l, r = map(int, s.strip().split(','))
  count = 0
  for i in range(l, r+1):
    if is_prime(i):
      count += 1
  print(count)

def main():
  with open(sys.argv[1]) as f:
    for line in f:
      process(line)

if __name__ == '__main__':
  main()