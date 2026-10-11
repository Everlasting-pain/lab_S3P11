#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <windows.h>
using namespace std;

struct Account {
    int id;
    string owner;
    string type;
    string currency;
    double balance;
    float rate;
    int term;
};

bool parseLine(const string& line, Account& acc) {
    stringstream ss(line);
    string f[7];
    for (int i = 0; i < 7; i++) {
        if (!getline(ss, f[i], ';')) return false;
    }
    try {
        acc.id = stoi(f[0]);
        acc.owner = f[1];
        acc.type = f[2];
        acc.currency = f[3];
        acc.balance = stod(f[4]);
        acc.rate = stof(f[5]);
        acc.term = stoi(f[6]);
    } catch (...) {
        return false;
    }
    return true;
}

Account* readFromFile(const string& filename, int& n) {
    n = 0;
    ifstream fin(filename);
    if (!fin) {
        cout << "Файл " << filename << " не найден!" << endl;
        return nullptr;
    }
    string line;
    int lines = 0;
    while (getline(fin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty()) lines++;
    }
    if (lines == 0) {
        cout << "Файл пустой." << endl;
        return nullptr;
    }
    Account* arr = new Account[lines];
    fin.clear();
    fin.seekg(0);
    int lineNum = 0;
    while (getline(fin, line)) {
        lineNum++;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        if (parseLine(line, arr[n])) {
            n++;
        } else {
            cout << "Строка " << lineNum << " с ошибкой, пропущена." << endl;
        }
    }
    fin.close();
    cout << "Загружено записей: " << n << endl;
    return arr;
}

void printTable(const Account* arr, int n) {
    if (n == 0) {
        cout << "Данных нет." << endl;
        return;
    }
    cout << left
         << setw(4)  << "ID"
         << setw(18) << "Owner"
         << setw(10) << "Type"
         << setw(6)  << "Cur"
         << right
         << setw(13) << "Balance"
         << setw(8)  << "Rate%"
         << setw(7)  << "Term" << endl;
    cout << string(66, '-') << endl;
    cout << fixed << setprecision(2);
    for (int i = 0; i < n; i++) {
        cout << left
             << setw(4)  << arr[i].id
             << setw(18) << arr[i].owner
             << setw(10) << arr[i].type
             << setw(6)  << arr[i].currency
             << right
             << setw(13) << arr[i].balance
             << setw(8)  << arr[i].rate
             << setw(7)  << arr[i].term << endl;
    }
}

string readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    if (!s.empty() && s.back() == '\r') s.pop_back();
    return s;
}

int readInt(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos;
            int x = stoi(s, &pos);
            if (pos == s.size()) return x;
        } catch (...) {}
        cout << "Нужно целое число, попробуйте ещё раз." << endl;
    }
}

double readDouble(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos;
            double x = stod(s, &pos);
            if (pos == s.size()) return x;
        } catch (...) {}
        cout << "Нужно число (дробная часть через точку), попробуйте ещё раз." << endl;
    }
}

int findById(const Account* arr, int n, int id) {
    for (int i = 0; i < n; i++) {
        if (arr[i].id == id) return i;
    }
    return -1;
}

void editAccount(Account* arr, int n) {
    int id = readInt("Введите ID счёта для изменения: ");
    int i = findById(arr, n, id);
    if (i == -1) {
        cout << "Счёт с ID " << id << " не найден." << endl;
        return;
    }
    cout << "Enter - оставить как было." << endl;
    string s;

    s = readLine("Владелец [" + arr[i].owner + "]: ");
    if (!s.empty()) arr[i].owner = s;

    s = readLine("Тип [" + arr[i].type + "]: ");
    if (!s.empty()) arr[i].type = s;

    s = readLine("Валюта [" + arr[i].currency + "]: ");
    if (!s.empty()) arr[i].currency = s;

    s = readLine("Баланс [" + to_string(arr[i].balance) + "]: ");
    if (!s.empty()) {
        try { arr[i].balance = stod(s); } catch (...) { cout << "Неверное число, баланс не изменён." << endl; }
    }

    s = readLine("Ставка % [" + to_string(arr[i].rate) + "]: ");
    if (!s.empty()) {
        try { arr[i].rate = stof(s); } catch (...) { cout << "Неверное число, ставка не изменена." << endl; }
    }

    s = readLine("Срок, мес. [" + to_string(arr[i].term) + "]: ");
    if (!s.empty()) {
        try { arr[i].term = stoi(s); } catch (...) { cout << "Неверное число, срок не изменён." << endl; }
    }
    cout << "Счёт " << id << " изменён." << endl;
}

void addAccount(Account*& arr, int& n) {
    int newId = 1;
    for (int i = 0; i < n; i++) {
        if (arr[i].id >= newId) newId = arr[i].id + 1;
    }

    Account acc;
    acc.id = newId;
    acc.owner    = readLine("Владелец (латиницей): ");
    acc.type     = readLine("Тип (Deposit/Credit/Current): ");
    acc.currency = readLine("Валюта (RUB/USD/EUR): ");
    acc.balance  = readDouble("Баланс: ");
    acc.rate     = (float)readDouble("Ставка, %: ");
    acc.term     = readInt("Срок, мес.: ");

    Account* bigger = new Account[n + 1];
    for (int i = 0; i < n; i++) bigger[i] = arr[i];
    bigger[n] = acc;

    delete[] arr;
    arr = bigger;
    n++;
    cout << "Счёт добавлен, ID = " << newId << endl;
}

void deleteAccount(Account*& arr, int& n) {
    int id = readInt("Введите ID счёта для удаления: ");
    int k = findById(arr, n, id);
    if (k == -1) {
        cout << "Счёт с ID " << id << " не найден." << endl;
        return;
    }
    if (n == 1) {
        delete[] arr;
        arr = nullptr;
        n = 0;
        cout << "Счёт удалён, записей не осталось." << endl;
        return;
    }
    Account* smaller = new Account[n - 1];
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (i != k) smaller[j++] = arr[i];
    }
    delete[] arr;
    arr = smaller;
    n--;
    cout << "Счёт " << id << " удалён." << endl;
}

void calculations(const Account* arr, int n) {
    if (n == 0) {
        cout << "Данных нет." << endl;
        return;
    }
    const string cur[3] = {"RUB", "USD", "EUR"};
    double sum[3] = {0, 0, 0};
    double rateSum = 0;
    int depCount = 0;
    int maxI = 0;

    cout << fixed << setprecision(2);
    cout << "Доход по вкладам:" << endl;
    for (int i = 0; i < n; i++) {
        for (int c = 0; c < 3; c++) {
            if (arr[i].currency == cur[c]) sum[c] += arr[i].balance;
        }
        if (arr[i].balance > arr[maxI].balance) maxI = i;
        if (arr[i].type == "Deposit") {
            rateSum += arr[i].rate;
            depCount++;
            double income = arr[i].balance * arr[i].rate / 100 * arr[i].term / 12;
            cout << "  ID " << arr[i].id << " " << arr[i].owner << ": "
                 << income << " " << arr[i].currency << endl;
        }
    }
    if (depCount == 0) cout << "  вкладов нет" << endl;

    cout << "Сумма балансов по валютам:" << endl;
    for (int c = 0; c < 3; c++) {
        cout << "  " << cur[c] << ": " << sum[c] << endl;
    }
    if (depCount > 0) {
        cout << "Средняя ставка по вкладам: " << rateSum / depCount << "%" << endl;
    }
    cout << "Наибольший баланс: ID " << arr[maxI].id << " " << arr[maxI].owner
         << " - " << arr[maxI].balance << " " << arr[maxI].currency << endl;
}

void saveToFile(const Account* arr, int n) {
    string filename = readLine("Имя файла для сохранения: ");
    if (filename.empty()) {
        cout << "Имя файла не введено." << endl;
        return;
    }
    ofstream fout(filename);
    if (!fout) {
        cout << "Не удалось создать файл " << filename << endl;
        return;
    }
    fout << fixed << setprecision(2);
    for (int i = 0; i < n; i++) {
        fout << arr[i].id << ';'
             << arr[i].owner << ';'
             << arr[i].type << ';'
             << arr[i].currency << ';'
             << arr[i].balance << ';'
             << arr[i].rate << ';'
             << arr[i].term << endl;
    }
    fout.close();
    cout << "Сохранено записей: " << n << " в файл " << filename << endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int n = 0;
    Account* accounts = readFromFile("data.txt", n);

    while (true) {
        cout << endl
             << "===== Банковские счета =====" << endl
             << "1. Просмотр" << endl
             << "2. Изменить запись" << endl
             << "3. Добавить запись" << endl
             << "4. Удалить запись" << endl
             << "5. Вычисления" << endl
             << "6. Сохранить в новый файл" << endl
             << "0. Выход" << endl;
        int choice = readInt("Ваш выбор: ");

        switch (choice) {
            case 1: printTable(accounts, n); break;
            case 2: editAccount(accounts, n); break;
            case 3: addAccount(accounts, n); break;
            case 4: deleteAccount(accounts, n); break;
            case 5: calculations(accounts, n); break;
            case 6: saveToFile(accounts, n); break;
            case 0: {
                string ans = readLine("Сохранить данные перед выходом? (y/n): ");
                if (ans == "y" || ans == "Y") saveToFile(accounts, n);
                delete[] accounts;
                accounts = nullptr;
                cout << "До свидания!" << endl;
                return 0;
            }
            default:
                cout << "Нет такого пункта." << endl;
        }
    }
}