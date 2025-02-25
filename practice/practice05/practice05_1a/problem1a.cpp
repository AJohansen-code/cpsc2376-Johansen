#include <iostream>
#include <vector>
#include <limits>

int main() {
    std::vector<int> numbers;
    int input;

    std::cout << "Enter the integers (enter a non-integer to stop): "<< std::endl;
    while (std::cin >> input) {
        numbers.push_back(input);
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Elements in order of input: "<< std::endl;
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    int evenSum = 0;
    for (int num : numbers) {
        if (num % 2 == 0) {
            evenSum += num;
        }
    }

    std::cout << "Sum of even numbers: " << evenSum << std::endl;
    return 0;
}
