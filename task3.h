#ifndef TASK3_H
#define TASK3_H
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

// Функцыя для метода Гаусса с выбором
vector<double> gauss_with_pivot(vector<vector<double>> A, vector<double> b) {
    int n = 4; // Размер системы
    vector<double> x(n);
    
    // Прямой ход с выбором главного элемента
    for (int k = 0; k < n-1; k++) {
        // Поиск главного элемента
        int max_idx = k;
        for (int i = k+1; i < n; i++)
            if (abs(A[i][k]) > abs(A[max_idx][k]))
                max_idx = i;
        
        // Обмен строк
        swap(A[k], A[max_idx]);
        swap(b[k], b[max_idx]);
        
        // Прямой ход
        for (int i = k+1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; j++)
                A[i][j] -= factor * A[k][j];
            b[i] -= factor * b[k];
        }
    }
    
    // Обратный ход
    for (int i = n-1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i+1; j < n; j++)
            x[i] -= A[i][j] * x[j];
        x[i] /= A[i][i];
    }
    return x;
}

// Функцыя для LU-разложения
vector<double> lu_decomposition(vector<vector<double>> A, vector<double> b) {
    int n = 4;
    vector<vector<double>> L(n, vector<double>(n, 0));
    vector<vector<double>> U(n, vector<double>(n, 0));
    vector<double> x(n), y(n);
    
    // LU-разложение
    for (int i = 0; i < n; i++) {
        L[i][i] = 1; // Диагональ L
        for (int j = i; j < n; j++) {
            U[i][j] = A[i][j];
            for (int k = 0; k < i; k++)
                U[i][j] -= L[i][k] * U[k][j];
        }
        for (int j = i+1; j < n; j++) {
            L[j][i] = A[j][i];
            for (int k = 0; k < i; k++)
                L[j][i] -= L[j][k] * U[k][i];
            L[j][i] /= U[i][i];
        }
    }
    
    // Прямой ход: Ly = b
    for (int i = 0; i < n; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++)
            y[i] -= L[i][j] * y[j];
    }
    
    // Обратный ход: Ux = y
    for (int i = n-1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i+1; j < n; j++)
            x[i] -= U[i][j] * x[j];
        x[i] /= U[i][i];
    }
    return x;
}

// Функцыя для проверки точности
bool check_accuracy(const vector<double>& x1, const vector<double>& x2, double eps) {
    for (int i = 0; i < x1.size(); i++)
        if (abs(x1[i] - x2[i]) > eps)
            return false;
    return true;
}

void task3_variant3() {
    // Параметры варианта 3
    double M = 1.08, N = 0.22, P = -1.16;
    double eps = 1e-3;
    
    // Матрица коэффициентов
    vector<vector<double>> A = {
        {M, -0.04, 0.21, -18},
        {0.25, -1.23, N, -0.09},
        {-0.21, N, 0.8, -0.13},
        {0.15, -1.31, 0.06, P}
    };
    
    // Правая часть
    vector<double> b = {-1.24, P, 2.56, M};
    
    // Решение методом Гаусса
    vector<double> x_gauss = gauss_with_pivot(A, b);
    cout << "Решение методом Гаусса:\n";
    for (int i = 0; i < 4; i++)
        cout << "x" << i+1 << " = " << x_gauss[i] << "\n";
    
    // Решение методом LU
    vector<double> x_lu = lu_decomposition(A, b);
    cout << "Решение методом LU:\n";
    for (int i = 0; i < 4; i++)
        cout << "x" << i+1 << " = " << x_lu[i] << "\n";
    
    // Проверка точности
    if (check_accuracy(x_gauss, x_lu, eps))
        cout << "Решения совпадают с точностью " << eps << "\n";
    else
        cout << "Решения не совпадают!\n";
}
#endif