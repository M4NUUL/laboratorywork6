// Реализация AES-128 в режиме CBC с PKCS#7 паддингом
#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <sstream>
#include <array>
#include <algorithm>
#include <limits>
#include <stdexcept>

using namespace std;
using Byte = uint8_t; // Тип для байта
using Block = array<Byte, 16>; // Блок данных для AES (16 байт)

// S-box для подстановки в AES
static const Byte SBOX[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

// Обратный S-box для дешифрования
static const Byte INV_SBOX[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

// Раундовые константы для расширения ключа
static const Byte RCON[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

// Генерация всех раундовых ключей из исходного ключа
array<Block,11> getRoundKey(const Block& key) {
    array<array<Byte,4>,44> W;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            W[i][j] = key[i*4 + j];

    for (int i = 4; i < 44; ++i) {
        array<Byte,4> temp = W[i-1];
        if (i % 4 == 0) {
            Byte t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;
            for (int k = 0; k < 4; ++k)
                temp[k] = SBOX[temp[k]];
            temp[0] ^= RCON[i/4];
        }
        
        for (int k = 0; k < 4; ++k)
            W[i][k] = W[i-4][k] ^ temp[k];
    }

    array<Block,11> roundKeys = {};
    for (int round = 0; round < 11; ++round) {
        for (int byteIdx = 0; byteIdx < 16; ++byteIdx) {
            int wordIdx = round*4 + (byteIdx / 4);
            int offset  = byteIdx % 4;
            roundKeys[round][byteIdx] = W[wordIdx][offset];
        }
    }
    return roundKeys;
}

// Операция SubBytes (байтовая подстановка)
void SubBytes(Block& state) { for(auto& byte : state) byte = SBOX[byte]; }

// Операция ShiftRows (циклический сдвиг строк)
void ShiftRows(Block& state) {
    Block temp = state;
    state[1] = temp[5]; state[5] = temp[9]; state[9] = temp[13]; state[13] = temp[1];
    state[2] = temp[10]; state[6] = temp[14]; state[10] = temp[2]; state[14] = temp[6];
    state[3] = temp[15]; state[7] = temp[3]; state[11] = temp[7]; state[15] = temp[11];
}


// Операция MixColumns (перемешивание столбцов)
auto xtime = [](Byte x){ return Byte((x<<1)^((x&0x80)?0x1B:0)); };
void MixColumns(Block& state) {
    for(int c = 0; c < 4; ++c){
        int i = 4*c;
        Byte a0 = state[i], a1 = state[i+1], a2 = state[i+2], a3 = state[i+3];
        state[i]   = xtime(a0) ^ (xtime(a1)^a1) ^ a2 ^ a3;
        state[i+1] = a0 ^ xtime(a1) ^ (xtime(a2)^a2) ^ a3;
        state[i+2] = a0 ^ a1 ^ xtime(a2) ^ (xtime(a3)^a3);
        state[i+3] = (xtime(a0)^a0) ^ a1 ^ a2 ^ xtime(a3);
    }
}



// Операция AddRoundKey (XOR с раундовым ключом)
void AddRoundKey(Block& state, const Block& roundKey) {
    for(int i=0; i<16; ++i) state[i] ^= roundKey[i];
}

// Обратные операции для дешифрования
void InvSubBytes(Block& state) { for(auto& byte : state) byte = INV_SBOX[byte]; }
void InvShiftRows(Block& state) {
    Block temp = state;
    state[1] = temp[13]; state[5] = temp[1]; state[9] = temp[5]; state[13] = temp[9];
    state[2] = temp[10]; state[6] = temp[14]; state[10] = temp[2]; state[14] = temp[6];
    state[3] = temp[7]; state[7] = temp[11]; state[11] = temp[15]; state[15] = temp[3];
}

// Умножение в поле GF(2^8) для InvMixColumns
Byte gmul(Byte a, Byte b) {
    Byte p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        bool hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) a ^= 0x1B;
        b >>= 1;
    }
    return p;
}
void InvMixColumns(Block& state) {
    for(int c = 0; c < 4; ++c) {
        int i = 4*c;
        Byte a0 = state[i], a1 = state[i+1], a2 = state[i+2], a3 = state[i+3];
        state[i]   = gmul(0x0e, a0) ^ gmul(0x0b, a1) ^ gmul(0x0d, a2) ^ gmul(0x09, a3);
        state[i+1] = gmul(0x09, a0) ^ gmul(0x0e, a1) ^ gmul(0x0b, a2) ^ gmul(0x0d, a3);
        state[i+2] = gmul(0x0d, a0) ^ gmul(0x09, a1) ^ gmul(0x0e, a2) ^ gmul(0x0b, a3);
        state[i+3] = gmul(0x0b, a0) ^ gmul(0x0d, a1) ^ gmul(0x09, a2) ^ gmul(0x0e, a3);
    }
}

// Шифрование одного блока (16 байт)
Block EncryptBlock(Block state, const array<Block,11>& roundKeys) {
    AddRoundKey(state, roundKeys[0]);

    for (int round = 1; round <= 9; ++round) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, roundKeys[round]);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, roundKeys[10]);

    return state;
}

// Дешифрование одного блока (16 байт)
Block DecryptBlock(Block state, const array<Block,11>& roundKeys) {
    AddRoundKey(state, roundKeys[10]);

    for (int round = 9; round >= 1; --round) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, roundKeys[round]);
        InvMixColumns(state);
    }

    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, roundKeys[0]);

    return state;
}

// Добавление PKCS#7 паддинга к данным
vector<Byte> addPKCS7Padding(vector<Byte>& data) {
    size_t paddingSize = 16 - (data.size() % 16);
    Byte paddingValue = static_cast<Byte>(paddingSize);
    for (size_t i = 0; i < paddingSize; ++i) {
        data.push_back(paddingValue);
    }
    return data;
}

// Удаление PKCS#7 паддинга
vector<Byte> removePKCS7Padding(vector<Byte>& data) {
    if (data.empty()) {
        throw runtime_error("Невозможно убрать дополнение из пустых данных.");
    }
    Byte paddingValue = data.back();
    if (paddingValue == 0 || paddingValue > 16 || data.size() < paddingValue) {
        return data; 
    }
    for (size_t i = 0; i < paddingValue; ++i) {
        if (data[data.size() - 1 - i] != paddingValue) return data;
    }
    data.erase(data.end() - paddingValue, data.end());
    return data;
}

// Перевод hex-символа в число
static Byte hexVal(char c) {
    if      (c >= '0' && c <= '9') return c - '0';
    else if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    else if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

// Разделение hex-строки на блоки по 16 байт
vector<Block> cutText(const string& text) {
    vector<Byte> data;
    for (size_t i = 0; i + 1 < text.size(); i += 2) {
        Byte b = (hexVal(text[i]) << 4) | hexVal(text[i+1]);
        data.push_back(b);
    }

    if (data.size() % 16 != 0) {
        cout << "Предупреждение: размер шифротекста не кратен 16. Результат может быть некорректным." << endl;
    }

    vector<Block> blocks;
    for (size_t off = 0; off < data.size(); off += 16) {
        Block block{};
        copy_n(data.begin()+off, 16, block.begin());
        blocks.push_back(block);
    }

    return blocks;
}

// Шифрование текста в режиме CBC
vector<Byte> encrypt(const array<Byte, 16>& key, const array<Byte, 16>& IV, string& text) {
    vector<Byte> plain_bytes(text.begin(), text.end());
    addPKCS7Padding(plain_bytes);
    
    array<Block,11> roundKeys = getRoundKey(key);
    vector<Byte> zhifrText;
    zhifrText.reserve(plain_bytes.size());

    Block prev_block = IV;

    for (size_t i = 0; i < plain_bytes.size(); i += 16) {
        Block current_block;
        copy(plain_bytes.begin() + i, plain_bytes.begin() + i + 16, current_block.begin());

        for(int j=0; j<16; ++j) current_block[j] ^= prev_block[j];

        Block encrypted_block = EncryptBlock(current_block, roundKeys);
        
        zhifrText.insert(zhifrText.end(), encrypted_block.begin(), encrypted_block.end());
        prev_block = encrypted_block;
    }
    return zhifrText;
}

// Дешифрование текста в режиме CBC
string decrypt(const array<Byte, 16>& key, const array<Byte, 16>& IV, string& text) {
    vector<Block> textBlocks = cutText(text);
    array<Block,11> roundKeys = getRoundKey(key);
    vector<Byte> decrypted_bytes;
    decrypted_bytes.reserve(textBlocks.size() * 16);

    Block prev_block = IV;

    for (auto& cipher_block : textBlocks) {
        Block decrypted_block = DecryptBlock(cipher_block, roundKeys);

        for (size_t i = 0; i < 16; ++i) {
            decrypted_block[i] ^= prev_block[i];
        }
        
        decrypted_bytes.insert(decrypted_bytes.end(), decrypted_block.begin(), decrypted_block.end());
        prev_block = cipher_block;
    }
    
    removePKCS7Padding(decrypted_bytes);
    
    return string(decrypted_bytes.begin(), decrypted_bytes.end());
}

// Вывод данных в hex-строке
template<typename Container>
string printHex(const Container& text) {
    ostringstream os;
    for (auto i : text) {
        os << hex << setw(2) << setfill('0') << nouppercase << static_cast<int>(i);
    }
    return os.str();
}

// Сохранение данных в бинарный файл
template<typename Container>
void hexToFile(const string& name, const Container& data) {
    string filename = name + ".bin";
    ofstream out(filename, ios::binary);

    out.write(reinterpret_cast<const char*>(data.data()), data.size());
    cout << "Успешно записано в " << filename << endl;
}

// Генерация случайных ключа и IV
pair<array<Byte, 16>, array<Byte, 16>> gen_key_iv() {
    array<Byte, 16> key;
    array<Byte, 16> IV;
    
    random_device rd;

    for (int i = 0; i < 16; ++i) {
        key[i] = static_cast<Byte>(rd());
        IV[i] = static_cast<Byte>(rd());
    }

    return {key, IV};
}

// Получение текста для шифрования/дешифрования (из файла или консоли)
string getText(int method) {
    cout << "Текст будет введен\n1. Из файла\n2. В консоли\n    Ввод > ";
    int cmd = 0;
    while (true) {
        cin >> cmd;
        if (cmd == 1 || cmd == 2) {break;}
        else {cout << "Введена неверная команда!" << endl;}
    }
    string text;
    if (cmd == 1) { 
        while (true) {
            cout << "Путь к файлу > ";
            string path; cin >> path;
            if (method == 1) {
                ifstream in(path, ios::binary);
                if (in) {
                    vector<Byte> textc((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
                    text = printHex(textc);
                }
            } else {
                ifstream in(path);
                if (in) {
                    text = string((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
                }
            }
            if (text.empty()) {
                cout << "Файл пустой или не существует!" << endl;
            } else {
                break;
            }
        }
    } else if (cmd == 2) {
        while (true) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Введите текст (в одну строку!) и нажмите Enter." << endl << "    Ввод > ";
            getline(cin, text);
            if (text.empty()) {
                cout << "Ничего не введено!" << endl;
            } else {
                break;
            }
        }
    }
    return text;
}

// Получение ключа и IV (из файла или консоли)
pair<Block, Block> getKeyIv() {
    cout << "Ключ и IV будет введен\n1. Из файла (bin)\n2. В консоли (hex)\n    Ввод > ";
    int cmd = 0;
    while (true) {
        cin >> cmd;
        if (cmd == 1 || cmd == 2) {break;} 
        else {cout << "Введена неверная команда!" << endl;}
    }
    Block key, IV;
    if (cmd == 1) { 
        while (true) {
            cout << "Путь к файлу для ключа > ";
            string path; cin >> path;
            ifstream in(path, ios::binary);
            in.read(reinterpret_cast<char*>(key.data()), key.size());
            if (!in || in.gcount() != key.size()) {
                cout << "Файл пустой или не существует!" << endl;
            } else {
                break;
            }
        }
        while (true) {
            cout << "Путь к файлу для IV > ";
            string path; cin >> path;
            ifstream in(path, ios::binary);
            in.read(reinterpret_cast<char*>(IV.data()), IV.size());
            if (!in || in.gcount() != IV.size()) {
                cout << "Файл пустой или не существует!" << endl;
            } else {
                break;
            }
        }
    } else if (cmd == 2) {
        while (true) {
            cout << "Введите ключ (32 hex-символа) и нажмите Enter." << endl << "    Ввод > ";
            string keyhex;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, keyhex);
            if (keyhex.size() != 32) {
                cout << "Неккорректный ввод! Длина должна быть 32 символа." << endl;
                continue;
            } else {
                for (int i = 0; i < 16; ++i) {
                    key[i] = Byte((hexVal(keyhex[2*i]) << 4) |  hexVal(keyhex[2*i + 1]));
                }
                break;
            }
        }
        while (true) {
            cout << "Введите IV (32 hex-символа) и нажмите Enter." << endl << "    Ввод > ";
            string ivhex;
            getline(cin, ivhex);
            if (ivhex.size() != 32) {
                cout << "Неккорректный ввод! Длина должна быть 32 символа." << endl;
                continue;
            } else {
                for (int i = 0; i < 16; ++i) {
                    IV[i] = Byte((hexVal(ivhex[2*i]) << 4) |  hexVal(ivhex[2*i + 1]));
                }
                break;
            }
        }
    }
    return {key, IV};
}

// Главная функция: выбор режима, запуск шифрования/дешифрования
int main() {
    try {
        cout << "Шифровальная машина имени НГТУ\nДобро пожаловать!" << endl;
        cout << "Будем\n1. Шифровать\n2. Дешифровать\n    Ввод > ";
        int cmd; cin >> cmd;
        if (cmd == 1) {
            string text = getText(0);
            cout << endl << "Текст для шифрования: " << text << endl << endl;
            auto [key, IV] = gen_key_iv();
            cout << "KEY: " << printHex(key) << endl;
            hexToFile("key", key);
            cout << "IV: " << printHex(IV) << endl;
            hexToFile("IV", IV);

            vector<Byte> eText = encrypt(key, IV, text);
            cout << endl << "Зашифрованный текст:" << endl << endl << printHex(eText) << endl;
            hexToFile("eText", eText);
        } else if (cmd == 2) {
            string text = getText(1);
            cout << endl << "Текст для расшифрования: " << text << endl << endl;
            auto [key, IV] = getKeyIv();
            cout << "KEY: " << printHex(key) << endl;
            cout << "IV: " << printHex(IV) << endl;

            string dText = decrypt(key, IV, text);
            cout << endl << "Расшифрованный текст:" << endl << endl << dText << endl;

        } else {
            cout << "Введена неверная команда!" << endl;
        }
    } catch (const std::exception& ex) {
        cerr << "\n[ОШИБКА] " << ex.what() << endl;
        cerr << "Программа завершена с ошибкой. Проверьте корректность ввода и файлов." << endl;
        return 1;
    } catch (...) {
        cerr << "\n[ОШИБКА] Неизвестная ошибка!" << endl;
        cerr << "Программа завершена с ошибкой. Проверьте корректность ввода и файлов." << endl;
        return 1;
    }
    return 0;
}