#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <limits>

int main() {
    std::list<std::string> task;
    int choice;

    do {
        std::cout << "Menu:\n";
        std::cout << "1. Add task\n";
        std::cout << "2. Remove task\n";
        std::cout << "3. Display tasks\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";

        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                std::string newTask;
                std::cout << "Enter a new task: ";
                std::getline(std::cin, newTask);
                task.push_back(newTask);
                std::cout << "Task added.\n";
                break;
            }
            case 2: {
                if (task.empty()) {
                    std::cout << "No tasks to remove.\n";
                    break;
                } 
                   int taskNumber;
                   std::cout << "Enter the task number to remove: ";

                   while (!(std::cin >> taskNumber)) {
                       std::cout << "Invalid input. Please enter a number: ";
                       std::cin.clear();
                       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                   }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (taskNumber > 0 && taskNumber <= task.size()) {
                    auto it = std::next(task.begin(), taskNumber - 1);
                    task.erase(it);
                    std::cout << "Task removed.\n";}
                else {
                    std::cout << "Invalid task number.\n";
                    }
                    break;
                
            }
            case 3: {
                if (task.empty()) {
                    std::cout << "No tasks available.\n";
                    break;
                } else {
                    std::cout << "Tasks:\n";
                    int i = 1;
                    for (const auto& t : task) {
                        std::cout << i++ << ". " << t << "\n";
                    }
                }
                break;
            }
            case 0:
                std::cout << "You are leaving the program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
         std::cout << "\n";
    }while (choice != 0);

    return 0;
}