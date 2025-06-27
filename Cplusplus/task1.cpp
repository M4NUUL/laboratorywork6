#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <set>
#include <unordered_set>

using namespace std;

void t1() {
    int N;
    cout << "Введите размерность матрицы N (N x N): ";
    cin >> N;
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(2, vector<int>(N));

    srand(time(0)); // Инициализация генератора случайных чисел

    cout << "Исходная матрица A:\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 21 - 10; 
            cout << A[i][j] << "\t";
        }
        cout << "\n";
    }

    // Заполнение матрицы B диагоналями
    for (int i = 0; i < N; ++i) {
        B[0][i] = A[i][i];         // главная диагональ
        B[1][i] = A[i][N - 1 - i]; // побочная диагональ
    }

    // Вывод матрицы B
    cout << "\nМатрица B (две строки с диагоналями):\n";
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << B[i][j] << "\t";
        }
        cout << "\n";
    }
}

void t2() {
    const int M = 5;
    const int N = 6;
    vector<vector<int>> A(M, vector<int>(N));
    vector<int> all_elements;

    srand(time(0));

    // Генерация матрицы и сбор всех элементов
    cout << "Исходная матрица:\n";
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 41 - 20; // от -20 до 20
            cout << A[i][j] << "\t";
            all_elements.push_back(A[i][j]);
        }
        cout << "\n";
    }

    // Нахождение максимума и второго минимального
    int max_elem = *max_element(all_elements.begin(), all_elements.end());

    // Используем set для получения уникальных значений и сортировки
    set<int> unique_elements(all_elements.begin(), all_elements.end());
    int second_min = *unique_elements.begin();
    if (unique_elements.size() > 1) {
        auto it = unique_elements.begin();
        ++it;
        second_min = *it;
    }

    int replacement_value = max_elem - second_min;

    // Замена элементов на диагоналях
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j || i + j == N - 1) {
                A[i][j] = replacement_value;
            }
        }
    }

    // Вывод результата
    cout << "\nМатрица после замены диагональных элементов:\n";
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << A[i][j] << "\t";
        }
        cout << "\n";
    }
}

bool isValidSudoku(const vector<vector<char>>& board) {
    vector<unordered_set<char>> rows(6), cols(6), boxes(4);
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            char num = board[r][c];
            if (num == '.') continue;
            int boxIndex = (r / 3) * 2 + (c / 3);
            if (rows[r].count(num) || cols[c].count(num) || boxes[boxIndex].count(num))
                return false;
            rows[r].insert(num);
            cols[c].insert(num);
            boxes[boxIndex].insert(num);
        }
    }
    return true;
}

vector<vector<char>> inputBoard() {
    vector<vector<char>> board(6, vector<char>(6, '.'));
    cout << "Введите заполненные ячейки (строка столбец значение), от 1 до 6. Введите -1 для завершения:\n";
    while (true) {
        int r, c;
        char val;
        cout << ">> ";
        cin >> r;
        if (r == -1) break;
        cin >> c >> val;
        if (r < 1 || r > 6 || c < 1 || c > 6 || val < '1' || val > '9') {
            cout << "Неверный ввод. Повторите.\n";
            continue;
        }
        board[r - 1][c - 1] = val;
    }
    return board;
}

vector<vector<char>> generateRandomBoard(int filled = 10) {
    vector<vector<char>> board(6, vector<char>(6, '.'));
    srand(time(0));
    int attempts = 0;
    while (filled > 0 && attempts < 100) {
        int r = rand() % 6;
        int c = rand() % 6;
        char val = '1' + rand() % 6;
        if (board[r][c] != '.') continue;
        board[r][c] = val;
        if (!isValidSudoku(board)) {
            board[r][c] = '.';
        } else {
            --filled;
        }
        ++attempts;
    }
    return board;
}

void printBoard(const vector<vector<char>>& board) {
    cout << "\nТекущая доска:\n";
    for (const auto& row : board) {
        for (char c : row) cout << c << ' ';
        cout << '\n';
    }
}

void t3() {
    cout << "Выберите режим:\n1 — Ввод вручную\n2 — Случайная генерация\n>> ";
    int choice;
    cin >> choice;
    vector<vector<char>> board;
    if (choice == 1)
        board = inputBoard();
    else
        board = generateRandomBoard();

    printBoard(board);

    if (isValidSudoku(board))
        cout << "Доска корректна по правилам судоку.\n";
    else
        cout << "Нарушены правила судоку.\n";
}

int main() {
    cout << "Выберите задачу:\n1 — Задача 1\n2 — Задача 2\n3 — Задача 3\n>> ";
    int task;
    cin >> task;
    switch (task) {
        case 1: t1(); break;
        case 2: t2(); break;
        case 3: t3(); break;
        default: cout << "Неверный выбор задачи.\n"; break;
    }
    return 0;
}