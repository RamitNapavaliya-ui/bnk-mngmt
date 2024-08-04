// #include <bits/stdc++.h>
//  #include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

// Utility functions
string getCurrentDateTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt);
}

// Account Class
class Account {
private:
    string accountNumber;
    string name;
    string dob;
    string accountType;
    double balance;
    vector<string> transactionHistory;

public:
    Account() {}
    Account(string accNo, string nm, string db, string accType, double bal)
        : accountNumber(accNo), name(nm), dob(db), accountType(accType), balance(bal) {}

    void deposit(double amount) {
        balance += amount;
        transactionHistory.push_back("Deposit: " + to_string(amount) + " on " + getCurrentDateTime());
    }

    bool withdraw(double amount) {
        if (amount > balance) return false;
        balance -= amount;
        transactionHistory.push_back("Withdraw: " + to_string(amount) + " on " + getCurrentDateTime());
        return true;
    }

    void showAccount() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Name: " << name << endl;
        cout << "Date of Birth: " << dob << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Balance: " << fixed << setprecision(2) << balance << endl;
        cout << "Transaction History: " << endl;
        for (const auto& trans : transactionHistory) {
            cout << trans << endl;
        }
    }

    string getAccountNumber() const { return accountNumber; }
    string getName() const { return name; }
    string getDob() const { return dob; }
    string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }

    void modifyAccount(string nm, string accType) {
        name = nm;
        accountType = accType;
    }

    vector<string> getTransactionHistory() const {
        return transactionHistory;
    }

    friend ofstream& operator<<(ofstream& ofs, const Account& acc);
    friend ifstream& operator>>(ifstream& ifs, Account& acc);
};

ofstream& operator<<(ofstream& ofs, const Account& acc) {
    ofs << acc.accountNumber << endl;
    ofs << acc.name << endl;
    ofs << acc.dob << endl;
    ofs << acc.accountType << endl;
    ofs << acc.balance << endl;
    ofs << acc.transactionHistory.size() << endl;
    for (const auto& trans : acc.transactionHistory) {
        ofs << trans << endl;
    }
    return ofs;
}

ifstream& operator>>(ifstream& ifs, Account& acc) {
    size_t transCount;
    ifs >> acc.accountNumber;
    ifs >> acc.name;
    ifs >> acc.dob;
    ifs >> acc.accountType;
    ifs >> acc.balance;
    ifs >> transCount;
    ifs.ignore();
    acc.transactionHistory.clear();
    for (size_t i = 0; i < transCount; ++i) {
        string trans;
        getline(ifs, trans);
        acc.transactionHistory.push_back(trans);
    }
    return ifs;
}

// Bank Class
class Bank {
private:
    vector<Account> accounts;
    string dataFile;

    void saveToFile() {
        ofstream outFile(dataFile);
        for (const auto& acc : accounts) {
            outFile << acc;
        }
        outFile.close();
    }

    void loadFromFile() {
        ifstream inFile(dataFile);
        if (!inFile) return;
        Account acc;
        while (inFile >> acc) {
            accounts.push_back(acc);
        }
        inFile.close();
    }

public:
    Bank(string filename) : dataFile(filename) {
        loadFromFile();
    }

    ~Bank() {
        saveToFile();
    }

    void newAccount() {
        string accNo, name, dob, accType;
        double amount;
        cout << "\nEnter The account No. : "; cin >> accNo;
        cout << "\n\nEnter The Name of The account Holder : "; cin.ignore(); getline(cin, name);
        cout << "\nEnter The Date of Birth (dd-mm-yyyy) : "; cin >> dob;
        cout << "\nEnter Type of The account (C/S) : "; cin >> accType;
        cout << "\nEnter The Initial amount : "; cin >> amount;
        accounts.push_back(Account(accNo, name, dob, accType, amount));
        cout << "\n\nAccount Created Successfully!";
    }

    Account* findAccount(string accNo, string nm) {
        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo && acc.getName() == nm) {
                return &acc;
            }
        }
        return nullptr;
    }

    Account* findAccountByDob(string accNo, string db) {
        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNo && acc.getDob() == db) {
                return &acc;
            }
        }
        return nullptr;
    }

    void closeAccount(string accNo, string nm) {
        auto it = remove_if(accounts.begin(), accounts.end(), [&](Account& acc) {
            return acc.getAccountNumber() == accNo && acc.getName() == nm;
        });
        accounts.erase(it, accounts.end());
    }

    void showAllAccounts() const {
        for (const auto& acc : accounts) {
            acc.showAccount();
            cout << "-------------------------" << endl;
        }
    }

    void depositAmount(string accNo, string name, double amount) {
        Account* acc = findAccount(accNo, name);
        if (acc) {
            acc->deposit(amount);
            cout << "\n\n\tAmount Deposited Successfully!";
        } else {
            cout << "\n\n\tAccount Not Found!";
        }
    }

    void withdrawAmount(string accNo, string name, double amount) {
        Account* acc = findAccount(accNo, name);
        if (acc) {
            if (acc->withdraw(amount)) {
                cout << "\n\n\tAmount Withdrawn Successfully!";
            } else {
                cout << "\n\n\tInsufficient Balance!";
            }
        } else {
            cout << "\n\n\tAccount Not Found!";
        }
    }

    void modifyAccount(string accNo, string name) {
        Account* acc = findAccount(accNo, name);
        if (acc) {
            cout << "\n\n\tEnter New Name: "; cin.ignore(); getline(cin, name);
            string accType;
            cout << "\n\n\tEnter New Account Type (C/S): "; cin >> accType;
            acc->modifyAccount(name, accType);
            cout << "\n\n\tAccount Modified Successfully!";
        } else {
            cout << "\n\n\tAccount Not Found!";
        }
    }

    void login(string accNo, string dob) {
        Account* acc = findAccountByDob(accNo, dob);
        if (acc) {
            cout << "\n\n\tLogin Successful!";
            acc->showAccount();
        } else {
            cout << "\n\n\tLogin Failed!";
        }
    }
};

// Main Function
int main() {
    Bank bank("bankdata.dat");

    int choice;

    do {
        cout << "\n\n\tBank Management System";
        cout << "\n\n\t01. New Account";
        cout << "\n\t02. Deposit Amount";
        cout << "\n\t03. Withdraw Amount";
        cout << "\n\t04. Balance Enquiry";
        cout << "\n\t05. All Account Holder List";
        cout << "\n\t06. Close an Account";
        cout << "\n\t07. Modify an Account";
        cout << "\n\t08. Login";
        cout << "\n\t09. Exit";
        cout << "\n\n\tSelect Your Option (1-9): ";
        cin >> choice;
        string accNo, name, dob;
        double amount;
        switch (choice) {
        case 1:
            bank.newAccount();
            break;
        case 2:
            cout << "\n\n\tEnter The account No. : "; cin >> accNo;
            cout << "\n\n\tEnter The account Holder Name : "; cin.ignore(); getline(cin, name);
            cout << "\n\n\tEnter The amount to be deposited : "; cin >> amount;
            bank.depositAmount(accNo, name, amount);
            break;
        case 3:
            cout << "\n\n\tEnter The account No. : "; cin >> accNo;
            cout << "\n\n\tEnter The account Holder Name : "; cin.ignore(); getline(cin, name);
            cout << "\n\n\tEnter The amount to be withdrawn : "; cin >> amount;
            bank.withdrawAmount(accNo, name, amount);
            break;
        case 4:
            cout << "\n\n\tEnter The account No. : "; cin >> accNo;
            cout << "\n\n\tEnter The account Holder Name : "; cin.ignore(); getline(cin, name);
            {
                Account* acc = bank.findAccount(accNo, name);
                if (acc) {
                    acc->showAccount();
                } else {
                    cout << "\n\n\tAccount Not Found!";
                }
            }
            break;
        case 5:
            bank.showAllAccounts();
            break;
        case 6:
            cout << "\n\n\tEnter The account No. : "; cin >> accNo;
            cout << "\n\n\tEnter The account Holder Name : "; cin.ignore(); getline(cin, name);
            bank.closeAccount(accNo, name);
            cout << "\n\n\tAccount Closed Successfully!";
            break;
        case 7:
            cout << "\n\n\tEnter The account No. : "; cin >> accNo;
            cout << "\n\n\tEnter The account Holder Name : "; cin.ignore(); getline(cin, name);
            bank.modifyAccount(accNo, name);
            break;
        case 8:
            cout << "\n\n\tEnter The account No. : "; cin >> accNo;
            cout << "\n\n\tEnter The Date of Birth (dd-mm-yyyy) : "; cin >> dob;
            bank.login(accNo, dob);
            break;
        case 9:
            exit(0);
        default:
            cout << "\a";
        }
    } while (choice != 9);

    return 0;
}