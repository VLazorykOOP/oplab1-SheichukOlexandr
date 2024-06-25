#include <iostream>
#include <fstream>
#include <cmath>
#include <stdexcept>
#include <string>

using namespace std;

class FileNotFoundException : public runtime_error {
public:
    FileNotFoundException(const string& filename)
        : runtime_error("File not found: " + filename) {}
};

class DivisionByZeroException : public runtime_error {
public:
    DivisionByZeroException()
        : runtime_error("Division by zero") {}
};

double Tbl(const string& filename, double x) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    double xi, yi;
    while (file >> xi >> yi) {
        if (xi == x) {
            return yi;
        }
    }
    throw runtime_error("Value not found in table");
}

double U(double x) {
    return Tbl("dat1.dat", x);
}

double T(double x) {
    return Tbl("dat2.dat", x);
}

double Qnk(double x, double y) {
    if (x == 0 || y == 0) {
        throw DivisionByZeroException();
    }
    return (U(x) + T(y)) / (x * y);
}

double Rnk(double x, double y, double z) {
    return Qnk(x, y) + Qnk(y, z) + Qnk(z, x);
}

double fun(double x, double y, double z) {
    return x * Rnk(x, y, z) + y * Rnk(y, z, x) + z * Rnk(z, x, y);
}

double func(double u, double v, const string& text);

double Gnk(double x, double y, double z) {
    try {
        return fun(x, y, z);
    }
    catch (const DivisionByZeroException&) {
        cerr << "Warning: Division by zero encountered in Gnk function. Adjusting values..." << endl;
        if (x == 0) {
            return fun(1, y, z);
        }
        if (y == 0) {
            return fun(x, 1, z);
        }
        if (z == 0) {
            return fun(x, y, 1);
        }
        return 0;
    }
}

double func(double u, double v, const string& text) {
    double result = 0.0;
    if (text.empty()) {
        result = Gnk(u, v, 1.0);
    }
    else {
        result = Gnk(u, v, 0.0);
    }
    return result;
}

int main() {
    double x, y, z;
    string text;

    cout << "Enter values for x, y, z and a text: ";
    cin >> x >> y >> z;
    cin.ignore();
    getline(cin, text);

    try {
        double result = func(x, y, text);
        cout << "Result: " << result << endl;
    }
    catch (const FileNotFoundException& e) {
        cerr << "Error: " << e.what() << endl;
    }
    catch (const DivisionByZeroException& e) {
        cerr << "Error: " << e.what() << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
