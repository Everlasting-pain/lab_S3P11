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

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int n = 0;
    Account* accounts = readFromFile("data.txt", n);
    printTable(accounts, n);

    delete[] accounts;   
    accounts = nullptr;
    return 0;
}