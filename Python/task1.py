import random

def t1():
    N = int(input("Введите размерность матрицы N: "))  

    A = [[random.randint(-10, 10) for _ in range(N)] for _ in range(N)]


    print("Исходная матрица A:")
    for row in A:
        print(row)

    main_diag = [A[i][i] for i in range(N)]
    secondary_diag = [A[i][N - 1 - i] for i in range(N)]

    B = [main_diag, secondary_diag]


    print("\nНовая матрица B (диагонали):")
    for row in B:
        print(row)

def t2():
   M, N = int(input("Введите количество строк M: ")), int(input("Введите количество столбцов N: "))

# Генерация матрицы случайными числами от -20 до 20
A = [[random.randint(-20, 20) for _ in range(N)] for _ in range(M)]

print("Исходная матрица:")
for row in A:
    print(row)

# Сбор всех элементов в один список для поиска max и second_min
all_elements = [elem for row in A for elem in row]

# Поиск максимума и второго наименьшего
max_elem = max(all_elements)
sorted_unique = sorted(set(all_elements))
second_min = sorted_unique[1] if len(sorted_unique) > 1 else sorted_unique[0]  # если все числа одинаковые

replacement_value = max_elem - second_min

# Замена диагональных элементов
for i in range(M):
    for j in range(N):
        if i == j or i + j == N - 1:
            A[i][j] = replacement_value

print("\nМатрица после замены диагональных элементов:")
for row in A:
    print(row)
    
def is_valid_sudoku(board):
    rows = [set() for _ in range(6)]
    cols = [set() for _ in range(6)]
    boxes = [set() for _ in range(4)]

    for r in range(6):
        for c in range(6):
            num = board[r][c]
            if num == '.':
                continue
            if num in rows[r] or num in cols[c]:
                return False
            box_index = (r // 3) * 2 + (c // 3)
            if num in boxes[box_index]:
                return False
            rows[r].add(num)
            cols[c].add(num)
            boxes[box_index].add(num)
    return True

def input_board():
    board = [['.' for _ in range(6)] for _ in range(6)]
    print("Введите заполненные ячейки в формате: row col value (1-6). Введите 'end' для завершения.")
    while True:
        line = input(">> ")
        if line.lower() == 'end':
            break
        try:
            r, c, v = line.strip().split()
            r, c = int(r)-1, int(c)-1
            if not (0 <= r < 6 and 0 <= c < 6 and v in '123456789'):
                print("Ошибка координат или значения.")
                continue
            board[r][c] = v
        except:
            print("Ошибка ввода. Пример: 1 2 5")
    return board

def generate_random_board(filled_cells=10):
    board = [['.' for _ in range(6)] for _ in range(6)]
    attempts = 0
    while filled_cells > 0 and attempts < 100:
        r = random.randint(0, 5)
        c = random.randint(0, 5)
        v = str(random.randint(1, 6))
        if board[r][c] != '.':
            continue
        board[r][c] = v
        if not is_valid_sudoku(board):
            board[r][c] = '.'
        else:
            filled_cells -= 1
        attempts += 1
    return board

def print_board(board):
    print("\nТекущая доска:")
    for row in board:
        print(" ".join(row))
    print()

def t3():
    print("Выберите режим:")
    print("1 — Ввод вручную")
    print("2 — Случайная генерация")
    choice = input(">> ")
    if choice == '1':
        board = input_board()
    else:
        board = generate_random_board()

    print_board(board)

    if is_valid_sudoku(board):
        print("Доска корректна по правилам судоку.")
    else:
        print("Нарушены правила судоку.")
        
        
def main():
    print("Выберите задачу:")
    print("1. Задача 1")
    print("2. Задача 2")
    print("3. Задача 3")
    
    choice = input("Введите номер задачи (1-3): ")
    
    if choice == '1':
        t1()
    elif choice == '2':
        t2()
    elif choice == '3':
        t3()
    else:
        print("Неверный выбор. Пожалуйста, выберите 1, 2 или 3.")
        
main()
