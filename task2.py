import random
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad

# Преобразование байтов в hex-строку
def to_hex(data: bytes) -> str:
    return data.hex()

# Функция шифрования текста
def encrypt_text(input_text: str, key: bytes, iv: bytes) -> bytes:
    cipher = AES.new(key, AES.MODE_CBC, iv)
    padded = pad(input_text.encode('utf-8'), AES.block_size)
    ciphertext = cipher.encrypt(padded)
    return ciphertext

# Функция дешифрования текста
def decrypt_text(ciphertext: bytes, key: bytes, iv: bytes) -> str:
    cipher = AES.new(key, AES.MODE_CBC, iv)
    plaintext = unpad(cipher.decrypt(ciphertext), AES.block_size)
    return plaintext.decode('utf-8')

# Задание 2, вариант 3

def task2_variant3():
    input_text = input("Введи текст для шифрования: ")
    key = b'mysecretkey12345'  # 16 байт
    # Генерация случайного IV
    iv = bytes([random.randint(0, 255) for _ in range(16)])
    original_iv = iv  # сохраняем копию
    ciphertext = encrypt_text(input_text, key, iv)
    print("Зашифрованный текст (hex):", to_hex(ciphertext))
    decrypted = decrypt_text(ciphertext, key, original_iv)
    print("Расшифрованный текст:", decrypted)

if __name__ == "__main__":
    task2_variant3()