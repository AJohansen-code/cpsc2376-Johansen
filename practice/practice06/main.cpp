#include <iostream>
#include "mixedFraction.h"
#include "fraction.h"

int main() {
    Fraction currentFraction;
    int choice, n, d;

    while (true) {
        std::cout << "Current:" << currentFraction << "\n Options:\n"
            << "1. Enter, 2. Add, 3. Subtract, 4. Multiply, 5. Divide, 6. Mixed, 7. Clear, 8. Exit\n";
        std::cin >> choice;

        try {
            if (choice >= 1 && choice <= 5) {
                std::cout << "Numerator:"; std::cin >> n;
                std::cout << "Denominator:"; std::cin >> d;
            }

            switch (choice) {
            case 1:
                currentFraction = Fraction(n, d);
                break;
            case 2:
                currentFraction = currentFraction + Fraction(n, d);
                break;
            case 3:
                currentFraction = currentFraction - Fraction(n, d);
                break;
            case 4:
                currentFraction = currentFraction * Fraction(n, d);
                break;
            case 5:
                currentFraction = currentFraction / Fraction(n, d);
                break;
            case 6:
                std::cout << "Mixed: " <<MixedFraction(currentFraction) << std::endl;
                break;
            case 7:
                currentFraction = Fraction();
                break;
            case 8:
                return 0;
            default: std::cout << "Invalid choice\n";
            }
        } catch (std::invalid_argument& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}