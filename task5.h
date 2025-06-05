#ifndef TASK5_H
#define TASK5_H
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

using namespace std;

void flip_coins(vector<vector<int>>& board, int i, int j, int di, int dj) {
    board[i][j] ^= 1;
    board[i + di][j + dj] ^= 1;
}

bool is_chessboard(const vector<vector<int>>& board, int n, int m, int start) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (board[i][j] != ((i + j + start) % 2))
                return false;
    return true;
}

int min_flips_with_start(vector<vector<int>> board, int n, int m, int start) {
    int flips = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (board[i][j] != ((i + j + start) % 2)) {
                if (i + 1 < n) {
                    flip_coins(board, i, j, 1, 0);
                    flips++;
                } else if (j + 1 < m) {
                    flip_coins(board, i, j, 0, 1);
                    flips++;
                } else {
                    return -1; // Невозможно сделать ход
                }
            }
        }
    return is_chessboard(board, n, m, start) ? flips : -1;
}

void task5() {
    int n, m;
    cout << "Введи размеры щита N и M: ";
    cin >> n >> m;
    srand(time(0));
    vector<vector<int>> board(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            board[i][j] = rand() % 2;

    cout << "Исходный щит:\n";
    for (const auto& row : board) {
        for (int cell : row)
            cout << cell << " ";
        cout << "\n";
    }

    int res0 = min_flips_with_start(board, n, m, 0);
    int res1 = min_flips_with_start(board, n, m, 1);

    if (res0 == -1 && res1 == -1)
        cout << "Невозможно получить шахматный порядок\n";
    else if (res0 == -1)
        cout << "Минимальное число ходов: " << res1 << "\n";
    else if (res1 == -1)
        cout << "Минимальное число ходов: " << res0 << "\n";
    else
        cout << "Минимальное число ходов: " << min(res0, res1) << "\n";
}
#endif
