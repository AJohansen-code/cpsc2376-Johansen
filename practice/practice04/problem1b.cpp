#include <iostream>
#include <string>
#include <limits>

std::string greet(std::string name = "Guest", std::string prefix = "Hello") {
  return prefix + ", " + name + "!";
}

int main() {
    int choice;
    std::string name, prefix;

    do {
        std::cout << "Menu:\n";
        std::cout << "1. Default greeting\n";
        std::cout << "2. default greeting with name\n";
        std::cout << "3. Custom greeting with prefix\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";

        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << greet() << std::endl;
                break;
            case 2:
                std::cout << "Enter your name: ";
                std::getline(std::cin >> std::ws, name);
                std::cout << greet(name) << "\n";
                break;
            case 3:
                std::cout << "Enter your name: ";
                std::getline(std::cin >> std::ws, name); 
                std::cout << "Enter greeting prefix: ";
                std::getline(std::cin, prefix);
                std::cout << greet(name, prefix) << "\n";
                break;
            case 0:
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        if(std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        }
    } while (choice != 0);

    std::cout << "You are now leaving, have a good day!\n";
    return 0;
}