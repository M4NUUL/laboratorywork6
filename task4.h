#ifndef TASK4_H
#define TASK4_H
#include <iostream>
using namespace std;

// Функция для вычисления максимальной ширины дорожки
int calculate_path_width(long long n, long long m, long long t) {
    long long left = 0, right = min(n, m) / 2;
    int result = 0;

    while (left <= right) {
        long long mid = (left + right) / 2;
        long long inner_n = n - 2 * mid;
        long long inner_m = m - 2 * mid;
        long long tiles_needed = n * m - max(0LL, inner_n) * max(0LL, inner_m);

        if (tiles_needed <= t) {
            result = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return result;
}

void task4() {
    long long n, m, t;
    cout << "Введи размеры площади n и m: ";
    cin >> n >> m;
    cout << "Введи количество плиток t: ";
    cin >> t;

    int width = calculate_path_width(n, m, t);
    cout << "Максимальная ширина дорожки: " << width << "\n";
}
#endif
