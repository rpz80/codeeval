import sys

def is_prime(n):
  i = 2
  while i*i <= n:
    if n%i == 0:
      return False
    i += 1
  return True

def process(s):
  max = int(s.strip())
  result = ''
  for i in range(2, max):
    if is_prime(i):
      result += str(i) + ','
  print(result[:-1])

def main():
  with open(sys.argv[1]) as f:
    for line in f:
      process(line)

if __name__ == '__main__':
  main()