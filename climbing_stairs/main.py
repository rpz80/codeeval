import sys
import time

def fib(n):
  if n == 1:
    return 1
  if n == 2:
    return 2
  nm1 = 2
  nm2 = 1
  i = 2
  while i != n:
    tmp = nm1
    nm1 = nm1 + nm2
    nm2 = tmp
    i += 1
  return nm1

def do_proc(count, current, step, limit):
  current += step
  if current > limit:
    return
  elif current == limit:
    count[0] += 1
  else:
    do_proc(count, current, 1, limit)
    do_proc(count, current, 2, limit)

def process(s):
  n = int(s)
  count = [0]
  # do_proc(count, 0, 1, n)
    # do_proc(count, 0, 2, n)
  print(fib(n))

def main():
  with open(sys.argv[1]) as f:
    for line in f:
      process(line)
  
if __name__ == '__main__':
  start = time.time()
  main()
  print('Elapsed: {0}'.format(time.time() - start))