#include <iostream>  // Підключає стандартну бібліотеку для вводу та виводу.
#include <fstream>   // Підключає бібліотеку для роботи з файлами.
#include <cmath>     // Підключає математичну бібліотеку.
#include <stdexcept> // Підключає бібліотеку виняткових ситуацій.
#include <string>    // Підключає бібліотеку для роботи з рядками.

using namespace std; // Дозволяє використовувати всі елементи стандартного простору імен без "std::".

class FileNotFoundException : public runtime_error { // Визначення класу винятку для файлів, що не знайдені.
public:
    FileNotFoundException(const string& filename)
        : runtime_error("File not found: " + filename) {} // Конструктор, що ініціалізує виняток з повідомленням.
};

class DivisionByZeroException : public runtime_error { // Визначення класу для винятку ділення на нуль.
public:
    DivisionByZeroException()
        : runtime_error("Division by zero") {} // Конструктор для винятку ділення на нуль.
};

double Tbl(const string& filename, double x) { // Функція для пошуку значення у файлі.
    ifstream file(filename); // Відкриття файлу.
    if (!file.is_open()) {  // Перевірка, чи файл вдалося відкрити.
        throw FileNotFoundException(filename); // Генерація винятку, якщо файл не відкритий.
    }

    double xi, yi;
    while (file >> xi >> yi) { // Читання пар значень з файлу.
        if (xi == x) {  // Якщо знайдено відповідне значення x.
            return yi;  // Повернення y.
        }
    }
    throw runtime_error("Value not found in table"); // Генерація винятку, якщо значення не знайдено.
}

double U(double x) { // Функція U, що використовує Tbl для файлу dat1.dat.
    return Tbl("dat1.dat", x);
}

double T(double x) { // Функція T, що використовує Tbl для файлу dat2.dat.
    return Tbl("dat2.dat", x);
}

double Qnk(double x, double y) { // Функція для обчислення значення за формулою.
    if (x == 0 || y == 0) { // Перевірка умови ділення на нуль.
        throw DivisionByZeroException(); // Генерація винятку ділення на нуль.
    }
    return (U(x) + T(y)) / (x * y); // Обчислення та повернення результату.
}

double Rnk(double x, double y, double z) { // Функція для обчислення суми трьох Qnk.
    return Qnk(x, y) + Qnk(y, z) + Qnk(z, x);
}

double fun(double x, double y, double z) { // Функція для обчислення складнішого математичного виразу.
    return x * Rnk(x, y, z) + y * Rnk(y, z, x) + z * Rnk(z, x, y);
}

double func(double u, double v, const string& text); // Прототип функції func.

double Gnk(double x, double y, double z) { // Функція Gnk, яка викликає fun і обробляє винятки.
    try {
        return fun(x, y, z);
    }
    catch (const DivisionByZeroException&) { // Обробка винятку ділення на нуль.
        cerr << "Warning: Division by zero encountered in Gnk function. Adjusting values..." << endl;
        if (x == 0) {  // Корекція значень для уникнення ділення на нуль.
            return fun(1, y, z);
        }
        if (y == 0) {
            return fun(x, 1, z);
        }
        if (z == 0) {
            return fun(x, y, 1);
        }
        return 0;  // Якщо всі значення нульові, повернення 0.
    }
}

double func(double u, double v, const string& text) { // Функція для визначення, як викликати Gnk.
    double result = 0.0; // Ініціалізація результату.
    if (text.empty()) { // Перевірка, чи текстовий рядок порожній.
        result = Gnk(u, v, 1.0); // Виклик Gnk з 1.0 як третім параметром.
    }
    else {
        result = Gnk(u, v, 0.0); // Виклик Gnk з 0.0 як третім параметром.
    }
    return result; // Повернення результату.
}

int main() { // Головна функція програми.
    double x, y, z;
    string text;

    cout << "Enter values for x, y, z and a text: "; // Запит вхідних даних.
    cin >> x >> y >> z; // Введення x, y, z.
    cin.ignore(); // Ігнорування залишкового вводу.
    getline(cin, text); // Введення текстового рядка.

    try {
        double result = func(x, y, text); // Спроба виконати func і зберегти результат.
        cout << "Result: " << result << endl; // Виведення результату.
    }
    catch (const FileNotFoundException& e) { // Обробка винятку відсутності файлу.
        cerr << "Error: " << e.what() << endl;
    }
    catch (const DivisionByZeroException& e) { // Обробка винятку ділення на нуль.
        cerr << "Error: " << e.what() << endl;
    }
    catch (const exception& e) { // Обробка будь-яких інших винятків.
        cerr << "Error: " << e.what() << endl;
    }

    return 0; // Закінчення виконання програми.
}
