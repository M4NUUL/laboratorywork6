#include <iostream>
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "task5.h"

using namespace std;

int main() {
    int choice;
    cout << "Выберити задание (1-5): ";
    cin >> choice;

    switch (choice) {
        case 1:
            task1_variant4();
            break;
        case 2:
            task2();
            break;
        case 3:
            task3();
            break;
        case 4:
            task4_variant2();
            break;
        case 5:
            task5();
            break;
        default:
            cout << "Неправильный выбор!\n";
    }

    return 0;
}