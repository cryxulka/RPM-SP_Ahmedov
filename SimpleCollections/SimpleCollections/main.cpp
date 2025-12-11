// main.cpp
#include <iostream>
#include <chrono>
#include <locale>
#include "Vector.hpp"
#include "Stack.hpp"

using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif

    const size_t N = 100000;

    auto start = chrono::high_resolution_clock::now();
    Vector<int> vec;
    for (size_t i = 0; i < N; ++i) {
        vec.push_back(static_cast<int>(i));
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration_vec = chrono::duration_cast<chrono::microseconds>(end - start);

    start = chrono::high_resolution_clock::now();
    Stack<int> stk;
    for (size_t i = 0; i < N; ++i) {
        stk.push(static_cast<int>(i));
    }
    end = chrono::high_resolution_clock::now();
    auto duration_stk = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Первые 10 элементов Vector: ";
    for (size_t i = 0; i < 10 && i < vec.size(); ++i) {
        cout << vec.begin()[i] << " ";
    }
    cout << "\n";

    cout << "Первые 10 элементов Stack (сверху вниз): ";
    Stack<int> copy = stk;
    size_t count = 0;
    while (!copy.empty() && count < 10) {
        cout << copy.top() << " ";
        copy.pop();
        ++count;
    }
    cout << "\n";

    cout << "Время вставки " << N << " элементов:\n";
    cout << "Vector: " << duration_vec.count() << " мкс\n";
    cout << "Stack:  " << duration_stk.count() << " мкс\n";

    return 0;
}