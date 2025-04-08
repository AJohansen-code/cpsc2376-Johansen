#include "checkers.h"
#include <iostream>

int main() {
    Checkers game;

    std::cout << "Welcome to Checkers!" << std::endl;

    while (game.status() == Checkers::Status::ONGOING) {
        std::cout << game;
        int startRow, startCol, endRow, endCol;
        std::cout << "Enter your move (startRow startCol endRow endCol): ";
        std::cin >> startRow >> startCol >> endRow >> endCol;
        game.play(startRow, startCol, endRow, endCol);
    }

    std::cout << game;

    return 0;
}