#include <iostream>
#include <vector>


void printVector(const std::vector<int>& vec) {
    for (size_t i = 0; i < vec.size(); i++) {  
        std::cout << vec.at(i) << " ";
    }
        std::cout << std::endl;
        system("pause");
    }

void doubleVector(std::vector<int>& vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        vec.at(i) *= 2;
    }
}

int sumVector(const std::vector<int>& vec) {
    int sum = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        sum += vec.at(i);
    }
    return sum;
}

void printMultiples(const std::vector<int>& vec) {
    int multiple;
    std::cout << "Enter a value to find multiples: ";
    std::cin >> multiple;
    
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec.at(i) % multiple == 0) {
            std::cout << vec.at(i) << " ";
        }
    }
    std::cout << std::endl;
    system("pause");
}

int main() {
    std::vector<int> numbers;
    int choice; 

   while (true) {
        std::cout << "\nMenu\n";
        std::cout << "Enter 1 to add an item\n";
        std::cout << "Enter 2 to print the vector\n";
        std::cout << "Enter 3 to double the vector\n";
        std::cout << "Enter 4 Find the sum\n";
        std::cout << "Enter 5 print multiples of a value\n";
        std::cout << "Enter 6 to exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 6) {
          std::cout << "Goodbye\n";
            break;
        }

        switch (choice) { 
          case 1:{
            int num;
            std::cout << "Enter a number: ";
            std::cin >> num;
            numbers.push_back(num);
            break;
            }

          case 2:{
            printVector(numbers);
            break;
            }
          case 3:{
            doubleVector(numbers);
            break;
            }
          case 4:{
            int sum = sumVector(numbers);
            std::cout << "The sum is: " << sum << std::endl;
            break;
            }
          case 5:{
            printMultiples(numbers);
            break;
            }
          default:
            std::cout << "Invalid choice. Please try another choice\n";
        }
    }

    return 0;
}


