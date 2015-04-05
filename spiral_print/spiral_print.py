import sys

debug = 0

def dbg_print(*args):
    if debug:
        print(*args)

def top_row(matrix, coil, M, N):
    ret = []
    for j in range(coil, N-coil):
        ret.append(matrix[coil][j])
    return ret

def bottom_row(matrix, coil, M, N):
    ret = []
    for j in range(coil, N-coil):
        ret.append(matrix[M-coil-1][N-j-1])
    return ret

def right_col(matrix, coil, M, N):
    ret = []
    for i in range(coil+1, M - coil-1):
        ret.append(matrix[i][N-coil-1])
    return ret

def left_col(matrix, coil, M, N):
    ret = []
    for i in range(coil+1, M - coil-1):
        ret.append(matrix[M - i - 1][coil])
    return ret


def print_matrix(m, M, N):
    res = []
    coil = 0
    while True:
        r = top_row(m, coil, M, N)
        if len(r) > 0:
            res = res + r
            dbg_print('tr', r)
        else:
            break

        r = right_col(m, coil, M, N)
        if len(r) > 0:
            res += r
            dbg_print('rc', r)
        else:
            if M % 2 != 0:
                break
        
        r = bottom_row(m, coil, M, N)
        if len(r) > 0:
            res += r
            dbg_print('br', r, len(r))
            if len(r) == 1:
                break
        else:
            break

        r = left_col(m, coil, M, N)
        if len(r) > 0:
            res += r
            dbg_print('lc', r)
        else:
            break

        coil += 1
    print(' '.join(res))
    

def main():
    with open(sys.argv[1]) as f:
        for line in f:
            m, n, matrix_str = line.strip().split(';')
            m, n = int(m), int(n)
            matrix_strs = matrix_str.split(' ')
            matrix = []
            for i in range(m):
                tmp = []
                for j in range(n):
                    tmp.append(matrix_strs[i*n + j])
                matrix.append(tmp)
            print_matrix(matrix, m, n)

if __name__ == '__main__':
    main()
