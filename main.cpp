#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

struct Transaction {
    string type;
    string category;
    string description;
    double amount;
};

struct CategoryTotal {
    string category;
    double total;
};

const string FILE_NAME = "transactions.txt";

void loadTransactions(vector<Transaction>& transactions) {
    ifstream file(FILE_NAME);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        Transaction t;
        string amountText;

        getline(ss, t.type, '|');
        getline(ss, t.category, '|');
        getline(ss, t.description, '|');
        getline(ss, amountText);

        if (!amountText.empty()) {
            t.amount = stod(amountText);
            transactions.push_back(t);
        }
    }

    file.close();
}

void saveTransactions(const vector<Transaction>& transactions) {
    ofstream file(FILE_NAME);

    for (const Transaction& t : transactions) {
        file << t.type << "|"
             << t.category << "|"
             << t.description << "|"
             << t.amount << "\n";
    }

    file.close();
}

double calculateBalance(const vector<Transaction>& transactions) {
    double balance = 0.0;

    for (const Transaction& t : transactions) {
        if (t.type == "Income") {
            balance += t.amount;
        } else {
            balance -= t.amount;
        }
    }

    return balance;
}

void showSummary(const vector<Transaction>& transactions) {
    double totalIncome = 0.0;
    double totalExpenses = 0.0;

    vector<CategoryTotal> categories;

    for (const Transaction& t : transactions) {
        if (t.type == "Income") {
            totalIncome += t.amount;
        } else if (t.type == "Expense") {
            totalExpenses += t.amount;

            bool found = false;

            for (CategoryTotal& c : categories) {
                if (c.category == t.category) {
                    c.total += t.amount;
                    found = true;
                    break;
                }
            }

            if (!found) {
                categories.push_back({t.category, t.amount});
            }
        }
    }

    cout << fixed << setprecision(2);

    cout << "\n========== FINANCIAL SUMMARY ==========\n";
    cout << "Total Income:   $" << totalIncome << endl;
    cout << "Total Expenses: $" << totalExpenses << endl;
    cout << "Net Savings:    $" << totalIncome - totalExpenses << endl;

    cout << "\n========== EXPENSES BY CATEGORY ==========\n";

    if (categories.empty()) {
        cout << "No expenses recorded.\n";
    } else {
        for (const CategoryTotal& c : categories) {
            cout << c.category << ": $" << c.total << endl;
        }
    }
}

int main() {
    vector<Transaction> transactions;
    int choice = 0;

    loadTransactions(transactions);

    cout << "Loaded " << transactions.size()
         << " previous transactions.\n";

    while (choice != 6) {
        cout << "\n============================\n";
        cout << "   PERSONAL FINANCE TRACKER\n";
        cout << "============================\n";
        cout << "1. Add Income\n";
        cout << "2. Add Expense\n";
        cout << "3. View Balance\n";
        cout << "4. View Transaction History\n";
        cout << "5. Financial Summary\n";
        cout << "6. Exit\n";

        cout << "\nEnter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1 || choice == 2) {
            double amount;
            string category;
            string description;

            cout << "Enter amount: $";

            if (!(cin >> amount)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid amount.\n";
                continue;
            }

            if (amount <= 0) {
                cin.ignore(10000, '\n');
                cout << "Amount must be greater than zero.\n";
                continue;
            }

            cin.ignore(10000, '\n');

            cout << "Enter category: ";
            getline(cin, category);

            cout << "Enter description: ";
            getline(cin, description);

            Transaction t;

            t.type = (choice == 1) ? "Income" : "Expense";
            t.category = category;
            t.description = description;
            t.amount = amount;

            transactions.push_back(t);
            saveTransactions(transactions);

            cout << "Transaction saved successfully!\n";
        }
        else if (choice == 3) {
            cout << fixed << setprecision(2);
            cout << "\nCurrent Balance: $"
                 << calculateBalance(transactions) << endl;
        }
        else if (choice == 4) {
            cout << fixed << setprecision(2);
            cout << "\n========== TRANSACTION HISTORY ==========\n";

            if (transactions.empty()) {
                cout << "No transactions recorded yet.\n";
            } else {
                for (int i = 0; i < transactions.size(); i++) {
                    cout << "\nTransaction #" << i + 1 << endl;
                    cout << "Type: " << transactions[i].type << endl;
                    cout << "Category: " << transactions[i].category << endl;
                    cout << "Description: " << transactions[i].description << endl;
                    cout << "Amount: $" << transactions[i].amount << endl;
                }

                cout << "\nCurrent Balance: $"
                     << calculateBalance(transactions) << endl;
            }
        }
        else if (choice == 5) {
            showSummary(transactions);
        }
        else if (choice == 6) {
            saveTransactions(transactions);
            cout << "Data saved. Goodbye!\n";
        }
        else {
            cout << "Invalid choice. Please select 1-6.\n";
        }
    }

    return 0;
}