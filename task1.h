#ifndef TASK1_H
#define TASK1_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

// Пункт 1: Создание матрицы и извлечение диагоналей
void create_diagonal_matrix(int n) {
    vector<vector<int>> matrix(n, vector<int>(n));
    srand(time(0));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = rand() % 101;

    cout << "Исходная матрица:\n";
    for (auto& row : matrix) {
        for (int val : row) cout << val << " ";
        cout << "\n";
    }

    vector<vector<int>> diag_matrix(2, vector<int>(n));
    for (int i = 0; i < n; i++) {
        diag_matrix[0][i] = matrix[i][i];
        diag_matrix[1][i] = matrix[i][n - 1 - i];
    }

    cout << "Матрица с диагоналями:\n";
    for (auto& row : diag_matrix) {
        for (int val : row) cout << val << " ";
        cout << "\n";
    }
}

// Пункт 2: Модификация диагоналей
void modify_diagonals_matrix(int m, int n) {
    vector<vector<int>> matrix(m, vector<int>(n));
    srand(time(0));

    vector<int> all_values;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            int val = rand() % 101;
            matrix[i][j] = val;
            all_values.push_back(val);
        }

    sort(all_values.begin(), all_values.end());
    all_values.erase(unique(all_values.begin(), all_values.end()), all_values.end());

    if (all_values.size() < 2) {
        cout << "Недостаточно уникальных значений для замены.\n";
        return;
    }

    int max_val = all_values.back();
    int second_min = all_values[1];
    int replacement = max_val - second_min;

    cout << "Заменяем диагонали значением: " << replacement << "\n";

    for (int i = 0; i < min(m, n); i++) {
        matrix[i][i] = replacement;
        matrix[i][n - 1 - i] = replacement;
    }

    cout << "Модифицированная матрица:\n";
    for (auto& row : matrix) {
        for (int val : row) cout << val << " ";
        cout << "\n";
    }
}

// Пункт 3: Проверка судоку 6×6 (частично заполненного)
bool is_valid_block(const vector<vector<int>>& board, int start_row, int start_col) {
    set<int> seen;
    for (int i = start_row; i < start_row + 3; i++)
        for (int j = start_col; j < start_col + 3; j++)
            if (board[i][j] != 0) {
                if (seen.count(board[i][j])) return false;
                seen.insert(board[i][j]);
            }
    return true;
}

bool validate_sudoku_6x6(const vector<vector<int>>& board) {
    for (int i = 0; i < 6; i++) {
        set<int> row, col;
        for (int j = 0; j < 6; j++) {
            if (board[i][j] != 0) {
                if (row.count(board[i][j])) return false;
                row.insert(board[i][j]);
            }
            if (board[j][i] != 0) {
                if (col.count(board[j][i])) return false;
                col.insert(board[j][i]);
            }
        }
    }

    for (int i = 0; i < 6; i += 3)
        for (int j = 0; j < 6; j += 3)
            if (!is_valid_block(board, i, j)) return false;

    return true;
}

// Объединяющая функция
void task1_variant4() {
    int choice;
    cout << "\nВыбери пункт (1 - диагонали, 2 - замена диагоналей, 3 - проверка судоку): ";
    cin >> choice;

    if (choice == 1) {
        int n;
        cout << "Введи размер квадратной матрицы (N > 6): ";
        cin >> n;
        if (n <= 6) {
            cout << "Ошибка! N должен быть больше 6.\n";
            return;
        }
        create_diagonal_matrix(n);
    } else if (choice == 2) {
        int m, n;
        cout << "Введи размеры матрицы (MxN): ";
        cin >> m >> n;
        modify_diagonals_matrix(m, n);
    } else if (choice == 3) {
        cout << "Введи судоку 6x6 (незаполненные ячейки = 0):\n";
        vector<vector<int>> board(6, vector<int>(6));
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                cin >> board[i][j];
        if (validate_sudoku_6x6(board))
            cout << "Судоку валидно по правилам.\n";
        else
            cout << "Судоку НЕвалидно.\n";
    } else {
        cout << "Неверный выбор.\n";
    }
}

#endif
