#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

using namespace std;

// Функция для вывода ошибок OpenSSL
void handleErrors() {
    cerr << "Ошибка OpenSSL" << endl;
    exit(EXIT_FAILURE);
}

// Генерация ключа и IV на основе пароля с использованием SHA-256 (современный API)
void generateKeyIV(const string& password, unsigned char* key, unsigned char* iv) {
    const int keyLength = 32; // AES-256 требует 32 байта (256 бит)
    const int ivLength = 16;  // Размер IV для AES-CBC

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) handleErrors();

    // Хэшируем пароль с помощью SHA-256
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
        EVP_MD_CTX_free(mdctx);
        handleErrors();
    }

    if (EVP_DigestUpdate(mdctx, password.c_str(), password.size()) != 1) {
        EVP_MD_CTX_free(mdctx);
        handleErrors();
    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned int hashLen;
    if (EVP_DigestFinal_ex(mdctx, hash, &hashLen) != 1) {
        EVP_MD_CTX_free(mdctx);
        handleErrors();
    }

    // Копируем первые 32 байта хэша в ключ
    memcpy(key, hash, keyLength);

    // Генерируем IV из следующей части хэша
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
        EVP_MD_CTX_free(mdctx);
        handleErrors();
    }

    if (EVP_DigestUpdate(mdctx, hash, SHA256_DIGEST_LENGTH) != 1 ||
        EVP_DigestUpdate(mdctx, password.c_str(), password.size()) != 1) {
        EVP_MD_CTX_free(mdctx);
        handleErrors();
    }

    if (EVP_DigestFinal_ex(mdctx, hash, &hashLen) != 1) {
        EVP_MD_CTX_free(mdctx);
        handleErrors();
    }

    EVP_MD_CTX_free(mdctx);
    memcpy(iv, hash, ivLength);
}

// Шифрование файла
bool encryptFile(const string& inputFile, const string& outputFile, const string& password) {
    unsigned char key[32], iv[16];
    generateKeyIV(password, key, iv);

    // Открываем входной и выходной файлы
    ifstream inFile(inputFile, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть входной файл: " << inputFile << endl;
        return false;
    }

    ofstream outFile(outputFile, ios::binary);
    if (!outFile) {
        cerr << "Не удалось открыть выходной файл: " << outputFile << endl;
        return false;
    }

    // Записываем IV в начало зашифрованного файла
    outFile.write(reinterpret_cast<const char*>(iv), sizeof(iv));

    // Инициализация контекста шифрования
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    // Буферы для чтения и шифрования
    const size_t bufferSize = 4096;
    unsigned char inBuffer[bufferSize], outBuffer[bufferSize + EVP_MAX_BLOCK_LENGTH];
    int bytesRead, outLength;

    while (inFile.read(reinterpret_cast<char*>(inBuffer), bufferSize) || inFile.gcount() > 0) {
        bytesRead = static_cast<int>(inFile.gcount());

        if (EVP_EncryptUpdate(ctx, outBuffer, &outLength, inBuffer, bytesRead) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            handleErrors();
        }
        outFile.write(reinterpret_cast<const char*>(outBuffer), outLength);
    }

    // Финальное шифрование
    if (EVP_EncryptFinal_ex(ctx, outBuffer, &outLength) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }
    outFile.write(reinterpret_cast<const char*>(outBuffer), outLength);

    // Очистка
    EVP_CIPHER_CTX_free(ctx);
    inFile.close();
    outFile.close();

    cout << "Файл зашифрован: " << outputFile << endl;
    return true;
}

// Расшифровка файла
bool decryptFile(const string& inputFile, const string& outputFile, const string& password) {
    unsigned char key[32], iv[16];

    // Открываем входной (зашифрованный) файл
    ifstream inFile(inputFile, ios::binary);
    if (!inFile) {
        cerr << "Не удалось открыть входной файл: " << inputFile << endl;
        return false;
    }

    // Читаем IV из начала файла
    inFile.read(reinterpret_cast<char*>(iv), sizeof(iv));
    if (inFile.gcount() != sizeof(iv)) {
        cerr << "Не удалось прочитать IV из файла" << endl;
        return false;
    }

    generateKeyIV(password, key, iv);

    ofstream outFile(outputFile, ios::binary);
    if (!outFile) {
        cerr << "Не удалось открыть выходной файл: " << outputFile << endl;
        return false;
    }

    // Инициализация контекста расшифрования
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        cerr << "Ошибка создания контекста" << endl;
        return false;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        cerr << "Ошибка инициализации дешифрования" << endl;
        return false;
    }

    const size_t bufferSize = 4096;
    unsigned char inBuffer[bufferSize], outBuffer[bufferSize + EVP_MAX_BLOCK_LENGTH];
    int bytesRead, outLength;

    while (inFile.read(reinterpret_cast<char*>(inBuffer), bufferSize) || inFile.gcount() > 0) {
        bytesRead = static_cast<int>(inFile.gcount());

        if (EVP_DecryptUpdate(ctx, outBuffer, &outLength, inBuffer, bytesRead) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            cerr << "Ошибка дешифрования" << endl;
            return false;
        }
        outFile.write(reinterpret_cast<const char*>(outBuffer), outLength);
    }

    // Финальная расшифровка
    if (EVP_DecryptFinal_ex(ctx, outBuffer, &outLength) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        cerr << "Ошибка финального дешифрования: неверный пароль или повреждённый файл" << endl;
        return false;
    }
    outFile.write(reinterpret_cast<const char*>(outBuffer), outLength);

    EVP_CIPHER_CTX_free(ctx);
    inFile.close();
    outFile.close();

    cout << "Файл расшифрован: " << outputFile << endl;
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "=== Программа шифрования/расшифровки файлов (AES-256-CBC) ===" << endl;
    cout << "1. Шифровать файл" << endl;
    cout << "2. Расшифровать файл" << endl;
    cout << "Выберите действие: ";

    int choice;
    cin >> choice;
    cin.ignore();

    string inputFile, outputFile, password;

    cout << "Введите путь к входному файлу: ";
    getline(cin, inputFile);
    cout << "Введите путь к выходному файлу: ";
    getline(cin, outputFile);
    cout << "Введите пароль: ";
    getline(cin, password);

    bool success = false;

    if (choice == 1) {
        success = encryptFile(inputFile, outputFile, password);
    }
    else if (choice == 2) {
        success = decryptFile(inputFile, outputFile, password);
    }
    else {
        cerr << "Неверный выбор." << endl;
        return EXIT_FAILURE;
    }

    if (success) {
        cout << "Операция выполнена успешно." << endl;
    }
    else {
        cerr << "Операция не выполнена." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}