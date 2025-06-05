from task1 import task1_variant4
from task2 import task2_variant3
from task3 import task3_variant3
from task4 import task4_variant2
from task5 import task5

def main():
    try:
        choice = int(input("Выберити задание (1-5): "))
    except ValueError:
        print("Неправильный выбор!")
        return
    if choice == 1:
        task1_variant4()
    elif choice == 2:
        task2_variant3()
    elif choice == 3:
        task3_variant3()
    elif choice == 4:
        task4_variant2()
    elif choice == 5:
        task5()
    else:
        print("Неправильный выбор!")

if __name__ == "__main__":
    main()
