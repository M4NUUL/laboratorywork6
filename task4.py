def calculate_path_width(n, m, t):
    left, right = 0, min(n, m) // 2
    result = 0
    while left <= right:
        mid = (left + right) // 2
        inner_n = n - 2 * mid
        inner_m = m - 2 * mid
        tiles_needed = n * m - max(0, inner_n) * max(0, inner_m)
        if tiles_needed <= t:
            result = mid
            left = mid + 1
        else:
            right = mid - 1
    return result

def task4_variant2():
    n = int(input("Введи размер площади n: "))
    m = int(input("Введи размер площади m: "))
    t = int(input("Введи количество плиток t: "))
    width = calculate_path_width(n, m, t)
    print(f"Максимальная ширина дорожки: {width}")

if __name__ == "__main__":
    task4_variant2()
