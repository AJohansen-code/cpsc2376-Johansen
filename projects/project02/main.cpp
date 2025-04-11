#include "checkers.h"
#include <iostream>
#include <limits>


int main() {
    Checkers game;

    std::cout << "Welcome to Checkers!" << std::endl;

    while (game.status() == Checkers::Status::ONGOING) {
        std::cout << game;
        int startRow, startCol, endRow, endCol;
        std::cout << "Enter your move (startRow startCol endRow endCol): ";

        if (std::cin >> startRow >> startCol >> endRow >> endCol){
           if (!game.play(startRow, startCol, endRow, endCol)){
    } else {
            std::cout << "Invalid input. Please enter four integers." << std::endl;
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        }
    }
}

    std::cout << game;

    return 0;
}
