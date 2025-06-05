import random

def create_diagonal_matrix(n):
    # создаём квадратную матрицу n x n
    matrix = [[random.randint(0, 100) for _ in range(n)] for _ in range(n)]
    print("Исходная матрица:")
    for row in matrix:
        print(' '.join(str(x) for x in row))
    # создаём матрицу 2 x n с главной и побочной диагоналями
    diag_matrix = [[0]*n for _ in range(2)]
    for i in range(n):
        diag_matrix[0][i] = matrix[i][i]  # главная диагональ
        diag_matrix[1][i] = matrix[i][n-1-i]  # побочная диагональ
    print("Матрица с диагоналями:")
    for row in diag_matrix:
        print(' '.join(str(x) for x in row))

def task1_variant4():
    n = int(input("Введи размер матрицы (N > 6): "))
    if n <= 6:
        print("Ошибка! N должен быть больше 6.")
        return
    create_diagonal_matrix(n)

if __name__ == "__main__":
    task1_variant4()
