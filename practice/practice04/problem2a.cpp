#include <iostream>
#include <string>
#include <limits>

template <typename T>
T calculate(T num1, T num2, char op) {
    switch (op) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 == 0) {
                throw std::runtime_error("Error: Division by zero.\n");
            }
            return num1 / num2;
        default:
            throw std::runtime_error("Error: Invalid operator.\n");
    }
}

int main() {
    std::string num1_str, num2_str;
    char op;

    std::cout << "Enter first number: ";
    std::cin >> num1_str;

    std::cout << "Enter second number: ";
    std::cin >> num2_str;

    std::cout << "Enter operator (+, -, *, /): ";
    std::cin >> op;

    try{
        double num1, num2;
        if (num1_str.find('.') != std::string::npos || num2_str.find('.') != std::string::npos) {
             num1 = std::stod(num1_str);
             num2 = std::stod(num2_str);
            double result = calculate(num1, num2, op);
            std::cout << "Result: " << result << std::endl;
        }   
        else {
            int num1 = std::stoi(num1_str);
            int num2 = std::stoi(num2_str);
            int result = calculate(num1, num2, op);
            std::cout << "Result: " << result << std::endl;
        }
    } catch (const std::runtime_error& error) {
        std::cerr << error.what();
        return 1;
    } catch (const std::invalid_argument& error) {
        std::cerr << "Error: Invalid input. Please enter numeric values.\n";
        return 1;
    } catch (const std::out_of_range& error) {
        std::cerr << "Error: Number is either too large or too small.\n";
        return 1;
    }
        
    return 0;
}