def calculate(n, m, t):
    if n <= 0 or m <= 0 or t <= 0:
        raise ValueError("Введённые данные должны быть положительными!")
    weight = 0
    while n > 2 and m > 2:
        line = 4 + (n - 2) * 2 + (m - 2) * 2
        if t >= line:
            t -= line
            n -= 2
            m -= 2
            weight += 1
        else:
            break
    return weight

def main():
    try:
        n, m = map(int, input("Введите размеры площади через пробел\n").split())
        t = int(input("Введите количество имеющихся плиток\n"))
        result = calculate(n, m, t)
        print(f"Максимально доступная ширина дорожки: {result}")
    except ValueError as e:
        print(e)

if __name__ == "__main__":
    main()