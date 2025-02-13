#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class BankAccount {
private:
    string accountHolder;
    int accountNumber;
    double balance;

public:
    // Constructor
    BankAccount(string name, int accNum, double initialBalance) {
        accountHolder = name;
        accountNumber = accNum;
        balance = initialBalance;
    }

    // Getters
    string getAccountHolder() const { return accountHolder; }
    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    // Deposit Money
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "?" << amount << " deposited successfully!\n";
        } else {
            cout << "Invalid deposit amount.\n";
        }
    }

    // Withdraw Money
    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "?" << amount << " withdrawn successfully!\n";
        } else {
            cout << "Insufficient funds or invalid amount.\n";
        }
    }

    // Display Account Details
    void displayAccountDetails() const {
        cout << "\n===== Account Details =====\n";
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Current Balance: ?" << fixed << setprecision(2) << balance << endl;
    }

    // Save to File
    void saveToFile(ofstream &outfile) const {
        outfile << accountHolder << " " << accountNumber << " " << balance << endl;
    }

    // Load from File
    static BankAccount loadFromFile(ifstream &infile) {
        string name;
        int accNum;
        double bal;
        infile >> ws;
        getline(infile, name, ' ');
        infile >> accNum >> bal;
        return BankAccount(name, accNum, bal);
    }
};

// Function to check if account number is unique
bool isAccountNumberUnique(vector<BankAccount> &accounts, int accNum) {
    for (const auto &acc : accounts) {
        if (acc.getAccountNumber() == accNum) {
            return false;
        }
    }
    return true;
}

// Function to find account by number
BankAccount* findAccount(vector<BankAccount>& accounts, int accNum) {
    for (auto &account : accounts) {
        if (account.getAccountNumber() == accNum) {
            return &account;
        }
    }
    return nullptr;
}

// Function to save all accounts to file
void saveAccountsToFile(vector<BankAccount>& accounts) {
    ofstream outfile("accounts.txt", ios::trunc);
    for (const auto &acc : accounts) {
        acc.saveToFile(outfile);
    }
    outfile.close();
}

// Function to load all accounts from file
void loadAccountsFromFile(vector<BankAccount>& accounts) {
    ifstream infile("accounts.txt");
    if (!infile) return; // If file doesn't exist, return

    while (!infile.eof()) {
        string name;
        int accNum;
        double bal;

        infile >> ws;
        if (getline(infile, name, ' ') && infile >> accNum >> bal) {
            accounts.push_back(BankAccount(name, accNum, bal));
        }
    }
    infile.close();
}

int main() {
    vector<BankAccount> accounts;
    loadAccountsFromFile(accounts); // Load existing accounts

    int choice;

    while (true) {
        cout << "\n===== Banking Management System =====\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Display Account Details\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            int accNum;
            double initialBalance;

            cout << "Enter Account Holder Name: ";
            cin.ignore();
            getline(cin, name);

            // Unique Account Number Check
            do {
                cout << "Enter Unique Account Number: ";
                cin >> accNum;
                if (!isAccountNumberUnique(accounts, accNum)) {
                    cout << "Error! Account number already exists. Try another.\n";
                }
            } while (!isAccountNumberUnique(accounts, accNum));

            cout << "Enter Initial Deposit: ?";
            cin >> initialBalance;

            accounts.push_back(BankAccount(name, accNum, initialBalance));
            saveAccountsToFile(accounts);
            cout << "Account Created Successfully!\n";

        } else if (choice == 2) {
            int accNum;
            double amount;

            cout << "Enter Account Number: ";
            cin >> accNum;
            BankAccount* acc = findAccount(accounts, accNum);

            if (acc) {
                cout << "Enter Deposit Amount: ?";
                cin >> amount;
                acc->deposit(amount);
                saveAccountsToFile(accounts);
            } else {
                cout << "Account not found!\n";
            }

        } else if (choice == 3) {
            int accNum;
            double amount;

            cout << "Enter Account Number: ";
            cin >> accNum;
            BankAccount* acc = findAccount(accounts, accNum);

            if (acc) {
                cout << "Enter Withdrawal Amount: ?";
                cin >> amount;
                acc->withdraw(amount);
                saveAccountsToFile(accounts);
            } else {
                cout << "Account not found!\n";
            }

        } else if (choice == 4) {
            int accNum;
            cout << "Enter Account Number: ";
            cin >> accNum;
            BankAccount* acc = findAccount(accounts, accNum);

            if (acc) {
                acc->displayAccountDetails();
            } else {
                cout << "Account not found!\n";
            }

        } else if (choice == 5) {
            cout << "Exiting Banking System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
