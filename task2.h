#ifndef TASK2_H
#define TASK2_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <openssl/aes.h>

using namespace std;

// Преобразование байтов в hex-строку
string to_hex(const unsigned char* data, int len) {
    stringstream ss;
    ss << hex << setfill('0');
    for (int i = 0; i < len; i++)
        ss << setw(2) << (int)data[i];
    return ss.str();
}

// Добавление PKCS#7 паддинга
void add_padding(vector<unsigned char>& data, int block_size = 16) {
    int padding_len = block_size - (data.size() % block_size);
    for (int i = 0; i < padding_len; ++i)
        data.push_back(padding_len);
}

// Удаление PKCS#7 паддинга
void remove_padding(vector<unsigned char>& data) {
    if (data.empty()) return;
    int padding_len = data.back();
    if (padding_len > 0 && padding_len <= 16 && padding_len <= data.size())
        data.resize(data.size() - padding_len);
}

// Функция шифрования текста
void encrypt_text(const string& input, vector<unsigned char>& ciphertext, const unsigned char* key, unsigned char* iv) {
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);

    vector<unsigned char> padded_input(input.begin(), input.end());
    add_padding(padded_input); // Добавляем паддинг

    ciphertext.resize(padded_input.size());
    AES_cbc_encrypt(padded_input.data(), ciphertext.data(), padded_input.size(), &aes_key, iv, AES_ENCRYPT);
}

// Функция дешифрования текста
string decrypt_text(const vector<unsigned char>& ciphertext, const unsigned char* key, unsigned char* iv) {
    AES_KEY aes_key;
    AES_set_decrypt_key(key, 128, &aes_key);

    vector<unsigned char> plaintext(ciphertext.size());
    AES_cbc_encrypt(ciphertext.data(), plaintext.data(), ciphertext.size(), &aes_key, iv, AES_DECRYPT);

    remove_padding(plaintext); // Удаляем паддинг

    return string(plaintext.begin(), plaintext.end());
}

// Задание
void task2() {
    string input;
    cout << "Введи текст для шифрования: ";
    cin.ignore(); // Очищаем буфер
    getline(cin, input);

    const unsigned char key[16] = { 'm','y','s','e','c','r','e','t','k','e','y','1','2','3','4','5' };

    // Генерация случайного IV
    unsigned char iv[16];
    unsigned char original_iv[16]; // сохраняем копию
    srand(time(0));
    for (int i = 0; i < 16; i++) {
        iv[i] = rand() % 256;
        original_iv[i] = iv[i];
    }

    vector<unsigned char> ciphertext;
    encrypt_text(input, ciphertext, key, iv);

    cout << "Зашифрованный текст (hex): " << to_hex(ciphertext.data(), ciphertext.size()) << "\n";

    string decrypted = decrypt_text(ciphertext, key, original_iv);
    cout << "Расшифрованный текст: " << decrypted << "\n";
}

#endif
