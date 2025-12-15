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

    const size_t N = 1000000;

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

        cout << "Vector: " << duration_vec_first.count() << "нс\n";
        cout << "Stack:  " << duration_stk_first.count() << "нс\n";
    }

    {
        Vector<int> vec;
        vec.reserve(N); 
        for (size_t i = 0; i < N - 1; ++i) {
            vec.push_back(static_cast<int>(i));
        }

        Stack<int> stk;

        for (size_t i = 0; i < N - 1; ++i) {
            stk.push(static_cast<int>(i));
        }

        auto start = chrono::high_resolution_clock::now();
        vec.push_back(static_cast<int>(N - 1));
        auto end = chrono::high_resolution_clock::now();
        auto duration_vec_last = chrono::duration_cast<chrono::nanoseconds>(end - start);

        start = chrono::high_resolution_clock::now();
        stk.push(static_cast<int>(N - 1));
        end = chrono::high_resolution_clock::now();
        auto duration_stk_last = chrono::duration_cast<chrono::nanoseconds>(end - start);

        cout << "Vector: " << duration_vec_last.count() << " нс\n";
        cout << "Stack:  " << duration_stk_last.count() << " нс\n";
    }

    return 0;
}