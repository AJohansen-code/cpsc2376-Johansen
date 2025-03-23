#include <iostream>
#include <string>

int main() {
    float redPotion{0.0f};
    float bluePotion{0.0f};
    float* flask{nullptr};

    std::string input;
    while (true) {
        std::cout << "Which potion to add liquid to (red/blue) or 'done' to quit: ";
        std::cin >> input;

        if (input == "red") {
            flask = &redPotion;
        } else if (input == "blue") {
            flask = &bluePotion;
        } else if (input == "done") {
            break;
        } else {
            std::cout << "Invalid input. Please enter 'red', 'blue', or 'done'." << std::endl;
            continue;
        }

        
        if (flask){
            float amount;
            std::cout << "How many milliliters of potion to add? ";
            std::cin >> amount;
            *flask += amount;
        }

        std::cout << "mL, Red Potion:" << redPotion << "mL, Blue Potion:" << bluePotion << "mL" << std::endl;
    }

    return 0;
}