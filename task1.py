import random
import os
import time

def z1(M, N):
    matrix = [[0.0 for _ in range(N)] for _ in range(M)]
    gen = random.Random()
    dis = lambda: gen.uniform(-50, 50)

    print("Исходная матрица:")
    for i in range(M):
        for j in range(N):
            matrix[i][j] = dis()
            print(matrix[i][j], end=" ")
        print()

    matrix.reverse()
    print("\nМатрица после переворота строк:")
    for row in matrix:
        for num in row:
            print(num, end=" ")
        print()

    unique_elements = set()
    for row in matrix:
        unique_elements.update(row)

    sorted_elements = sorted(unique_elements)
    second_min = sorted_elements[1]
    second_max = sorted_elements[-2]

    print(f"\nВторой минимальный элемент: {second_min}")
    print(f"Второй максимальный элемент: {second_max}")

def z2(N):
    chet = (N % 2 == 0)
    if chet:
        N2 = N // 2
    else:
        N2 = (N + 1) // 2

    matrix = [[0 for _ in range(N2)] for _ in range(N2)]
    chisl = 100
    for i in range(N2):
        for j in range(i + 1):
            matrix[i][j] = chisl
            chisl += 5

    for row in matrix:
        for elem in row:
            if elem != 0:
                print(elem, end=" ")
        print()

    matrix.reverse()
    for row in matrix:
        if not chet:
            chet = True
            continue
        for elem in row:
            if elem != 0:
                print(elem, end=" ")
        print()

life = []

def initLife(I, J):
    global life
    print("Делаем случайное(1) поле или устойчивую фигуру(2)?")
    cmd = int(input())
    if cmd == 1:
        gen = random.Random()
        life = [[ "#" if gen.randint(0,1) == 1 else "." for _ in range(J)] for _ in range(I)]
    elif cmd == 2:
        life = [["." for _ in range(J)] for _ in range(I)]
        life[0][0] = "#"
        life[0][1] = "#"
        life[1][0] = "#"
        life[1][1] = "#"

def pixellife(i, j, prav):
    count = 0
    for di in (-1, 0, 1):
        for dj in (-1, 0, 1):
            if di == 0 and dj == 0:
                continue
            ni, nj = i + di, j + dj
            if 0 <= ni < len(life) and 0 <= nj < len(life[0]):
                if life[ni][nj] == "#":
                    count += 1

    if prav == 1:
        if life[i][j] == ".":
            return "#" if count == 3 else "."
        else:
            return "." if (count < 2 or count > 3) else "#"
    elif prav == 2:
        if life[i][j] == ".":
            return "#" if (count % 2 == 0 and count != 0) else "."
        else:
            return "#" if (count % 2 == 1) else "."
    else:
        return "X"

def printlife():
    for row in life:
        print("".join(row))
    print()

def lifegame(I, J):
    global life
    initLife(I, J)

    print("Играем по:\n"
          "1. Cтандартным правилам (Живая умирает когда соседей <2 и >3, мертвая воскресает при 3 живых соседях)\n"
          "2. Измененным (Живая выживет если кол-во соседей нечётное, мертвая воскреснет если кол-во соседей чётное)\n>>> ", end="")
    prav = int(input())

    while True:
        os.system("clear")
        printlife()

        nextFrame = [[None for _ in range(J)] for _ in range(I)]
        for i in range(I):
            for j in range(J):
                nextFrame[i][j] = pixellife(i, j, prav)

        life = nextFrame
        time.sleep(1)

def main():
    print("Какое задание демонстрировать? > ", end="")
    cmd = int(input())
    print()
    if cmd == 1:
        print("Введите через пробел M и N размеры матрицы")
        M, N = map(int, input().split())
        z1(M, N)
    elif cmd == 2:
        print("Введите количество (N) строк")
        N = int(input())
        z2(N)
    elif cmd == 3:
        print("Введите через пробел размеры поля")
        I, J = map(int, input().split())
        lifegame(I, J)
    else:
        print("Введена неверная команда!")

if __name__ == "__main__":
    main()