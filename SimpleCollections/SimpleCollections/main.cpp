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

    const size_t N = 10000000;
    const size_t LAST_ELEMENTS_COUNT = 1000;

    {
        auto start = chrono::high_resolution_clock::now();
        Vector<int> vec;
        vec.push_back(1);
        auto end = chrono::high_resolution_clock::now();
        auto duration_vec_first = chrono::duration_cast<chrono::nanoseconds>(end - start);

        start = chrono::high_resolution_clock::now();
        Stack<int> stk;
        stk.push(1);
        end = chrono::high_resolution_clock::now();
        auto duration_stk_first = chrono::duration_cast<chrono::nanoseconds>(end - start);

        cout << "Vector: " << duration_vec_first.count() << "HC\n";
        cout << "Stack: " << duration_stk_first.count() << "HC\n";
    }

    {
        Vector<int> vec;
        vec.reserve(N + 1000);
        for (size_t i = 0; i < N - LAST_ELEMENTS_COUNT; ++i) {
            vec.push_back(static_cast<int>(i));
        }

        Stack<int> stk;
        for (size_t i = 0; i < N - LAST_ELEMENTS_COUNT; ++i) {
            stk.push(static_cast<int>(i));
        }

        auto start = chrono::high_resolution_clock::now();
        for (size_t i = N - LAST_ELEMENTS_COUNT; i < N; ++i) {
            vec.push_back(static_cast<int>(i));
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration_vec_last = chrono::duration_cast<chrono::nanoseconds>(end - start);

        start = chrono::high_resolution_clock::now();
        for (size_t i = N - LAST_ELEMENTS_COUNT; i < N; ++i) {
            stk.push(static_cast<int>(i));
        }
        end = chrono::high_resolution_clock::now();
        auto duration_stk_last = chrono::duration_cast<chrono::nanoseconds>(end - start);

        cout << "full Vector: " << duration_vec_last.count() << " HC\n";
        cout << "full Stack: " << duration_stk_last.count() << " HC\n";
    }

    return 0;
}