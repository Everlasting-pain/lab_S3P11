#include <iostream>
#include <windows.h>
#include <string>
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
int main() {
    SetConsoleOutputCP(CP_UTF8);
    cout << "Окей гугл что такое С++" << endl;
    return 0;
}
