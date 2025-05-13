#ifndef CHECKERS_H
#define CHECKERS_H

#include <SDL2/SDL.h>
#include <vector>
#include <ostream>

struct Point {
    int x;
    int y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class Checkers {
public:
    enum class Piece : int {
        EMPTY = 0,
        BLACK = 1,
        RED = 2,
        BLACK_KING = 3,
        RED_KING = 4
    };

    enum class Player : int {
        RED = 1,
        BLACK = 2
    };

    inline friend std::ostream& operator<<(std::ostream& os, Checkers::Player player) {
        switch (player) {
        case Checkers::Player::RED:
            os << "RED";
            break;

        case Checkers::Player::BLACK:
            os << "BLACK";
            break;
        default:
            os << "UNKNOWN";
            break;
        }
        return os;
    }

    enum class Status {
        ONGOING,
        RED_WINS,
        BLACK_WINS,
        DRAW
    };

    static constexpr int BOARD_SIZE = 8;
    Checkers();
    ~Checkers() = default;

    void initializeBoard();

    void draw(SDL_Renderer* renderer, int squareSize) const;

    void handleInput(const SDL_Event& event, int squareSize);

    std::vector<Point> getPotentialMoves(int row, int col, Piece piece, bool onlyJumps = false) const;

    bool hasForcedJump(Player player) const;

    Piece getPieceAt(int row, int col) const;

    void setPieceAt(int row, int col, Piece piece);

    bool isWithinBounds(int row, int col) const;

    Player getCurrentPlayer() const { return currentPlayer; }

    const Point& getSelectedPiece() const { return selectedPiece; }

private:
    std::vector<std::vector<Piece>> board;

    Player currentPlayer;

    Point selectedPiece{ -1, -1 };

    bool inJumpSequence = false;

    Point jumpingPiece{ -1, -1 };

    void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) const;

    void drawPiece(SDL_Renderer* renderer, Piece piece, int centerX, int centerY, int radius) const;
};

#endif // CHECKERS_H

