#include <iostream>
#include <forward_list>
#include <list>
#include <random>
#include <chrono>
#include <clocale>
#include <fstream>

using namespace std;

const int N = 1000000;
const int RANGE = 1001;

void fill_forward_list(forward_list<int>& fl, mt19937& gen) {
    fl.clear();
    uniform_int_distribution<> dis(0, RANGE - 1);
    for (int i = 0; i < N; ++i)
        fl.push_front(dis(gen));
}

void fill_list(list<int>& l, mt19937& gen) {
    l.clear();
    uniform_int_distribution<> dis(0, RANGE - 1);
    for (int i = 0; i < N; ++i)
        l.push_back(dis(gen));
}

int main() {
    setlocale(LC_ALL, "Russian");

    mt19937 gen(12345);

    forward_list<int> fl;
    list<int> l;

    cout << "Заполнение списков (1 000 000 элементов)...\n";
    fill_forward_list(fl, gen);
    fill_list(l, gen);
    cout << "Готово.\n\n";

    const int REPEATS = 1000;
    bool first_write = true;
    ifstream check("results.csv");
    if (check.good()) first_write = false;
    check.close();

    while (true) {
        cout << "Выберите операцию:\n";
        cout << "1. Вставка элемента в начало\n";
        cout << "2. Удаление первого элемента\n";
        cout << "3. Вставка элемента в конец\n";
        cout << "4. Удаление последнего элемента\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор (0–4): ";

        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "Завершение работы.\n";
            break;
        }

        auto start = chrono::high_resolution_clock::now();
        auto end = chrono::high_resolution_clock::now();
        double time_fl = -1, time_l = -1;
        string operation_name;

        if (choice == 1) {
            operation_name = "Вставка в начало";
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < REPEATS; ++i) fl.push_front(42);
            end = chrono::high_resolution_clock::now();
            time_fl = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count() / REPEATS;

            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < REPEATS; ++i) l.push_front(42);
            end = chrono::high_resolution_clock::now();
            time_l = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count() / REPEATS;
        }
        else if (choice == 2) {
            operation_name = "Удаление первого";
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < REPEATS; ++i) fl.pop_front();
            end = chrono::high_resolution_clock::now();
            time_fl = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count() / REPEATS;

            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < REPEATS; ++i) l.pop_front();
            end = chrono::high_resolution_clock::now();
            time_l = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count() / REPEATS;
        }
        else if (choice == 3) {
            operation_name = "Вставка в конец";
            time_fl = -1;
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < REPEATS; ++i) l.push_back(42);
            end = chrono::high_resolution_clock::now();
            time_l = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count() / REPEATS;
        }
        else if (choice == 4) {
            operation_name = "Удаление последнего";
            time_fl = -1;
            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < REPEATS; ++i) l.pop_back();
            end = chrono::high_resolution_clock::now();
            time_l = chrono::duration_cast<chrono::duration<double, micro>>(end - start).count() / REPEATS;
        }
        else {
            cout << "Неверный выбор. Попробуйте снова.\n\n";
            continue;
        }

        cout << "\n--- Результаты ---\n";
        cout << "Операция: " << operation_name << "\n";
        if (time_fl >= 0)
            cout << "forward_list: " << time_fl << " мкс\n";
        else
            cout << "forward_list: не поддерживается\n";
        cout << "list: " << time_l << " мкс\n\n";

        ofstream file("results.csv", ios::app);
        if (first_write) {
            file.imbue(locale("C"));
            file << "Операция;forward_list;list\n";
            first_write = false;
        }
        file.imbue(locale("C"));
        file << operation_name << ";";
        if (time_fl >= 0)
            file << time_fl << ";";
        else
            file << "-;";
        file << time_l << "\n";
        file.close();
    }

    return 0;
}