import numpy as np

def gauss_with_pivot(A, b):
    n = 4
    A = [row[:] for row in A]
    b = b[:]
    x = [0.0] * n
    for k in range(n-1):
        max_idx = max(range(k, n), key=lambda i: abs(A[i][k]))
        A[k], A[max_idx] = A[max_idx], A[k]
        b[k], b[max_idx] = b[max_idx], b[k]
        for i in range(k+1, n):
            factor = A[i][k] / A[k][k]
            for j in range(k, n):
                A[i][j] -= factor * A[k][j]
            b[i] -= factor * b[k]
    for i in range(n-1, -1, -1):
        x[i] = b[i]
        for j in range(i+1, n):
            x[i] -= A[i][j] * x[j]
        x[i] /= A[i][i]
    return x

def lu_decomposition(A, b):
    n = 4
    L = [[0.0]*n for _ in range(n)]
    U = [[0.0]*n for _ in range(n)]
    x = [0.0]*n
    y = [0.0]*n
    for i in range(n):
        L[i][i] = 1.0
        for j in range(i, n):
            U[i][j] = A[i][j]
            for k in range(i):
                U[i][j] -= L[i][k] * U[k][j]
        for j in range(i+1, n):
            L[j][i] = A[j][i]
            for k in range(i):
                L[j][i] -= L[j][k] * U[k][i]
            L[j][i] /= U[i][i]
    for i in range(n):
        y[i] = b[i]
        for j in range(i):
            y[i] -= L[i][j] * y[j]
    for i in range(n-1, -1, -1):
        x[i] = y[i]
        for j in range(i+1, n):
            x[i] -= U[i][j] * x[j]
        x[i] /= U[i][i]
    return x

def check_accuracy(x1, x2, eps):
    return all(abs(a-b) <= eps for a, b in zip(x1, x2))

def task3_variant3():
    M = 1.08
    N = 0.22
    P = -1.16
    eps = 1e-3
    A = [
        [M, -0.04, 0.21, -18],
        [0.25, -1.23, N, -0.09],
        [-0.21, N, 0.8, -0.13],
        [0.15, -1.31, 0.06, P]
    ]
    b = [-1.24, P, 2.56, M]
    x_gauss = gauss_with_pivot(A, b)
    print("Решение методом Гаусса:")
    for i, val in enumerate(x_gauss):
        print(f"x{i+1} = {val}")
    x_lu = lu_decomposition(A, b)
    print("Решение методом LU:")
    for i, val in enumerate(x_lu):
        print(f"x{i+1} = {val}")
    if check_accuracy(x_gauss, x_lu, eps):
        print(f"Решения совпадают с точностью {eps}")
    else:
        print("Решения не совпадают!")

if __name__ == "__main__":
    task3_variant3()
