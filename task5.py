import random

def flip_coins(board, i, j, di, dj):
    board[i][j] ^= 1
    board[i + di][j + dj] ^= 1

def is_chessboard(board, n, m, start):
    for i in range(n):
        for j in range(m):
            if board[i][j] != ((i + j + start) % 2):
                return False
    return True

def min_flips_with_start(board, n, m, start):
    board = [row[:] for row in board]
    flips = 0
    for i in range(n):
        for j in range(m):
            if board[i][j] != ((i + j + start) % 2):
                if i + 1 < n:
                    flip_coins(board, i, j, 1, 0)
                    flips += 1
                elif j + 1 < m:
                    flip_coins(board, i, j, 0, 1)
                    flips += 1
                else:
                    return -1
    return flips if is_chessboard(board, n, m, start) else -1

def task5():
    n = int(input("Введи размер щита N: "))
    m = int(input("Введи размер щита M: "))
    random.seed()
    board = [[random.randint(0, 1) for _ in range(m)] for _ in range(n)]
    print("Исходный щит:")
    for row in board:
        print(' '.join(str(cell) for cell in row))
    res0 = min_flips_with_start(board, n, m, 0)
    res1 = min_flips_with_start(board, n, m, 1)
    if res0 == -1 and res1 == -1:
        print("Невозможно получить шахматный порядок")
    elif res0 == -1:
        print(f"Минимальное число ходов: {res1}")
    elif res1 == -1:
        print(f"Минимальное число ходов: {res0}")
    else:
        print(f"Минимальное число ходов: {min(res0, res1)}")

if __name__ == "__main__":
    task5()
