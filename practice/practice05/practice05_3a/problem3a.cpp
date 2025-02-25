#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
using std::numeric_limits;

int main() {
    std::vector<int> numbers;
    int input;

    std::cout << "Enter integers (0 to stop): ";
    while (std::cin >> input && input != 0) {
        numbers.push_back(input);
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Squares of the elements:\n";
    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        std::cout << n * n << " ";
    });
    std::cout << std::endl;

    int sumOfSquares = std::accumulate(numbers.begin(), numbers.end(), 0, [](int sum, int n) {
        int square = n * n;
        return sum + square;
    });

    std::cout << "Sum of squares: " << sumOfSquares << std::endl;

    return 0;
}