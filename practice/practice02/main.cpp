#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

const std::string BALANCE_FILE = "account_balance.txt";

void writeBalanceToFile(double balance) {
    std::ofstream file(BALANCE_FILE);
    if (file.is_open()) {
        file << std::fixed << std::setprecision(2) << balance;
        file.close();
    } else {
        std::cerr << "ERROR: Unable to open file for writing." << std::endl;
        exit(1);
    }
}

double readBalanceFromFile() {
    std::ifstream file(BALANCE_FILE);
    double balance = 0.0;
    if (file.is_open()) {
        file >> balance;
        file.close();
    }
    else {
        std::cout << "Initialzing account with $100.00..." << std::endl;
        balance = 100.00;
        writeBalanceToFile(balance);
        system("pause");
    }
    return balance;
}

void checkBalance(double balance) {
    std::cout << "\nYour account balance is: $" << std::fixed << std::setprecision(2) << balance << std::endl;
}

void deposit(double &balance) {
    double amount;
    std::cout << "Enter deposit amount: ";
    std::cin >> amount;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "ERROR: Invalid input. Please enter a number." << std::endl;
    } else if (amount < 0) {
        std::cerr << "ERROR: Deposit amount must be positive." << std::endl;
    } else {
        balance += amount;
        writeBalanceToFile(balance);
        std::cout << "\nDeposit of $" << std::fixed << std::setprecision(2) << amount << " successful." << std::endl;
    }
}

void withdraw(double &balance) {
    double amount;
    std::cout << "Enter withdrawal amount: ";
    std::cin >> amount;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "ERROR: Invalid input. Please enter a number." << std::endl;
    } else if (amount < 0) {
        std::cerr << "ERROR: Withdrawal amount must be positive." << std::endl;
    } else if (amount > balance) {
        std::cerr << "ERROR: Insufficient funds." << std::endl;
    } else {
        balance -= amount;
        writeBalanceToFile(balance);
        std::cout << "\nWithdrawal of $" << std::fixed << std::setprecision(2) << amount << " successful." << std::endl;
    }
}


int main() {
    double balance = readBalanceFromFile();

    int choice;
    do {
        std::cout << "\n*Welcome to the Bank of Johansen*\n";
        std::cout << "\nMenu:\n";
        std::cout << "1. Check balance\n";
        std::cout << "2. Deposit Money\n";
        std::cout << "3. Withdraw Money\n";
        std::cout << "4. Exit\n";
        std::cout << "\nEnter choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "ERROR: Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                checkBalance(balance);
                break;
            case 2:
                deposit(balance);
                break;
            case 3:
                withdraw(balance);
                break;
            case 4:
                std::cout << "\nHave a good day!" << std::endl;
                break;
            default:
                std::cerr << "ERROR: Invalid choice. Please enter a number between 1 and 4." << std::endl;
        }
    } 
    while (choice != 4);

    return 0;
}

   







