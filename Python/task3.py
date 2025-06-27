import copy
import math

MAX_ITER = 10000
EPS = 1e-3

def gauss(A_in, b_in):
    n = len(A_in)
    A = copy.deepcopy(A_in)
    b = b_in[:]
    x = [0.0] * n

    # Прямой ход
    for k in range(n):
        pivotRow = k
        for i in range(k+1, n):
            if abs(A[i][k]) > abs(A[pivotRow][k]):
                pivotRow = i
        A[k], A[pivotRow] = A[pivotRow], A[k]
        b[k], b[pivotRow] = b[pivotRow], b[k]

        for i in range(k+1, n):
            factor = A[i][k] / A[k][k]
            for j in range(k, n):
                A[i][j] -= factor * A[k][j]
            b[i] -= factor * b[k]

    # Обратный ход
    for i in range(n-1, -1, -1):
        s = b[i]
        for j in range(i+1, n):
            s -= A[i][j] * x[j]
        x[i] = s / A[i][i]

    return x

def zeidel(A, b):
    n = len(A)
    x = [0.0] * n
    prev = [0.0] * n

    for _ in range(MAX_ITER):
        for i in range(n):
            if abs(A[i][i]) < 1e-12:
                print("Итерационный процесс не сошелся: нулевой диагональный элемент!")
                return []
            s = b[i]
            for j in range(i):
                s -= A[i][j] * x[j]
            for j in range(i+1, n):
                s -= A[i][j] * prev[j]
            x[i] = s / A[i][i]
            if math.isnan(x[i]):
                print("Итерационный процесс не сошелся: NaN!")
                return []

        eps = max(abs(x[i] - prev[i]) for i in range(n))
        if eps < EPS:
            return x
        prev = x[:]

    print(f"Итерационный процесс не сошелся за {MAX_ITER} итераций!")
    return []

def main():
    M = 0.93
    N = 0.07
    P = -0.84
    A = [
        [M,   -0.04, 0.21, -18],
        [0.25, -1.23, N,   -0.09],
        [-0.21, N,    0.8, -0.13],
        [0.15, -1.31, 0.06,  P]
    ]
    b = [-1.24, P, 2.56, M]

    print("Решение методом Гаусса:")
    sol_g = gauss(A, b)
    for val in sol_g:
        print(f"{val:6}", end=" ")
    print()

    print("Решение методом Зейделя:")
    sol_z = zeidel(A, b)
    for val in sol_z:
        print(f"{val:6}", end=" ")
    print()

if __name__ == "__main__":
    main()