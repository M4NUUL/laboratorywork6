#ifndef TASK1_H
#define TASK1_H
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

// Функцыя для создания матрицы с диагоналями
void create_diagonal_matrix(int n) {
    // Создаем квадратную матрицу NxN
    vector<vector<int>> matrix(n, vector<int>(n));
    srand(time(0));
    // Заполняем матрицу случайными числами
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = rand() % 101; // Случайные числа [0,100]

    // Выводим исходную матрицу
    cout << "Исходная матрица:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << matrix[i][j] << " ";
        cout << "\n";
    }

    // Создаем матрицу 2xN с главной и побочной диагоналями
    vector<vector<int>> diag_matrix(2, vector<int>(n));
    // Извлекаем диагонали
    for (int i = 0; i < n; i++) {
        diag_matrix[0][i] = matrix[i][i]; // Главная диагональ
        diag_matrix[1][i] = matrix[i][n-1-i]; // Побочная диагональ
    }

    // Выводим матрицу с диагоналями
    cout << "Матрица с диагоналями:\n";
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < n; j++)
            cout << diag_matrix[i][j] << " ";
        cout << "\n";
    }
}

void task1_variant4() {
    int n;
    cout << "Введи размер матрицы (N > 6): ";
    cin >> n;
    if (n <= 6) {
        cout << "Ошибка! N должен быть больше 6.\n";
        return;
    }
    create_diagonal_matrix(n);
}
#endif