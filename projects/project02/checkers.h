#ifndef CHECKERS_H
#define CHECKERS_H

#include <vector>
#include <iostream>
#include <utility> // for std::pair

class Checkers {
public:
    enum class Piece : char {
        EMPTY = '.',
        WHITE = 'w',
        BLACK = 'b',
        WHITE_KING = 'W',
        BLACK_KING = 'B'
    };

    enum class Player {
        WHITE,
        BLACK
    };

    enum class Status {
        ONGOING,
        WHITE_WINS,
        BLACK_WINS,
        DRAW
    };

private:
    static const int BOARD_SIZE = 8;
    std::vector<std::vector<Piece>> board;
    Player currentPlayer;
    Status gameStatus;

    bool isValidMove(int startRow, int startCol, int endRow, int endCol) const;
    bool isJumpMove(int startRow, int startCol, int endRow, int endCol) const;
    std::vector<std::pair<int, int>> getPossibleJumps (int row, int col) const;
    std::vector<std::pair<int, int>> getPossibleMoves(int row, int col) const;
    bool canForceJump(Player player) const;
    void makeMove(int startRow, int startCol, int endRow, int endCol);
    void makeJump(int startRow, int startCol, int endRow, int endCol);
    void updateKings();
    void updateStatus();
    Piece getPieceAt(int row, int col) const;
    void setPieceAt(int row, int col, Piece piece);

public:
    Checkers();
    bool play(int startRow, int startCol, int endRow, int endCol);
    Status status() const;
    void display() const;

    friend std::ostream& operator<<(std::ostream& os, const Checkers& game);
};

#endif // CHECKERS_H
