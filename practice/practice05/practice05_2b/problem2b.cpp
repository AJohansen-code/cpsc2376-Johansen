#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

int main() {
    std::vector<int> numbers;
    int number;

    std::cout << "Enter a integer number(enter a non-interger to stop); \n";

    while (std::cin >> number) {
        numbers.push_back(number);
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (numbers.empty()) {
        std::cout << "No numbers were entered. \n";
        return 1;
    }

    auto max_element_it = std::max_element(numbers.begin(), numbers.end());
    auto min_element_it = std::min_element(numbers.begin(), numbers.end());

    if (max_element_it != numbers.end()) {
        std::cout << "Lagest number: " << *max_element_it << std::endl;
    } else {
        std::cout << "Error finding the largest element." << std::endl;
    }

    if (min_element_it != numbers.end()) {
        std::cout << "Smallest number: " << *min_element_it << std::endl;
    } else {
        std::cout << "Error finding the smallest element." << std::endl;
    }

    return 0;
}