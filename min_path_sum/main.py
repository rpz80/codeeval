import sys

def sum_elem(i,j, sum, results, matrix, msize):
  sum += matrix[i][j]

  if i == msize - 1 and j == msize - 1:
    results.append(sum)
  else:
    if i + 1 != msize:
      sum_elem(i+1, j, sum, results, matrix, msize)
    if j + 1 != msize:
      sum_elem(i, j+1, sum, results, matrix, msize)


def process(s, f):
  matrix_size = int(s.strip())
  matrix = []

  for i in range(matrix_size):
    matrix.append(list(map(int, f.readline().strip().split(','))))
  
  results = []
  sum_elem(0, 1, matrix[0][0], results, matrix, matrix_size)
  sum_elem(1, 0, matrix[0][0], results, matrix, matrix_size)

  print(min(results))


def main():
  with open(sys.argv[1]) as f:
    for line in f:
      process(line, f)

if __name__ == '__main__':
  main()