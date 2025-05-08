#include "checkers.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>

const int BOARD_SIZE = 8;
const int SQUARE_SIZE = 60;

void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius;  x++) {
			if ((x * x + y * y) <= (radius * radius)) {
				SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
			}
		}
	}
}

void Checkers::draw(SDL_Renderer* renderer, int selectedRow, int selectedCol, std::pair<int, int> startPos) {
    SDL_Color brown = { 139, 69, 19, 255 };
    SDL_Color lightBrown = { 222, 184, 135, 255 };
    SDL_Color red = { 255, 0, 0, 255 };
    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Color highlight = { 0, 255, 0, 255 }; // Green for highlighting
    SDL_Color startHighlight = { 0, 0, 255, 255 }; // Blue for start position


    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            SDL_Rect square;
            square.x = col * SQUARE_SIZE;
            square.y = row * SQUARE_SIZE;
            square.w = SQUARE_SIZE;
            square.h = SQUARE_SIZE;

            if ((row + col) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, lightBrown.r, lightBrown.g, lightBrown.b, lightBrown.a);
            }
            else {
                SDL_SetRenderDrawColor(renderer, brown.r, brown.g, brown.b, brown.a);
            }
            SDL_RenderFillRect(renderer, &square);
        }
    }
    if (selectedRow != -1 && selectedCol != -1) {
        SDL_Rect highlightRect;
        highlightRect.x = selectedCol * SQUARE_SIZE;
        highlightRect.y = selectedRow * SQUARE_SIZE;
        highlightRect.w = SQUARE_SIZE;
        highlightRect.h = SQUARE_SIZE;
        SDL_SetRenderDrawColor(renderer, highlight.r, highlight.g, highlight.b, highlight.a);
        SDL_RenderDrawRect(renderer, &highlightRect);
    }

    // Highlight the starting square
    if (startPos.first != -1 && startPos.second != -1) {
        SDL_Rect startHighlightRect;
        startHighlightRect.x = startPos.second * SQUARE_SIZE;
        startHighlightRect.y = startPos.first * SQUARE_SIZE;
        startHighlightRect.w = SQUARE_SIZE;
        startHighlightRect.h = SQUARE_SIZE;
        SDL_SetRenderDrawColor(renderer, startHighlight.r, startHighlight.g, startHighlight.b, startHighlight.a);
        SDL_RenderDrawRect(renderer, &startHighlightRect);
    }

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            int centerX = col * SQUARE_SIZE + SQUARE_SIZE / 2;
            int centerY = row * SQUARE_SIZE + SQUARE_SIZE / 2;
            int pieceRadius = SQUARE_SIZE / 3;

            switch (board[row][col]) {
            case Piece::RED:
                SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
                drawFilledCircle(renderer, centerX, centerY, pieceRadius);
                break;
            case Piece::BLACK:
                SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
                drawFilledCircle(renderer, centerX, centerY, pieceRadius);
                break;
            case Piece::RED_KING:
                SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
                drawFilledCircle(renderer, centerX, centerY, pieceRadius);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for king mark
                drawFilledCircle(renderer, centerX, centerY, pieceRadius / 3);
                break;
            case Piece::BLACK_KING:
                SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
                drawFilledCircle(renderer, centerX, centerY, pieceRadius);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for king mark
                drawFilledCircle(renderer, centerX, centerY, pieceRadius / 3);
                break;
            case Piece::EMPTY:
            default:
                break;
            }
        }
    }

    std::string statusText;
    if (gameStatus == Status::ONGOING) {
        statusText = std::string("Current Player: ") + (currentPlayer == Player::RED ? "Red" : "Black");
    }
    else if (gameStatus == Status::RED_WINS) {
        statusText = "Red wins!";
    }
    else if (gameStatus == Status::BLACK_WINS) {
        statusText = "Black wins!";
    }
    else if (gameStatus == Status::DRAW) {
        statusText = "Draw!";
    }
    std::cout << statusText << std::endl;
}

Checkers::Checkers() : currentPlayer(Player::RED), gameStatus(Status::ONGOING) {
    board.resize(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE, Piece::EMPTY));

    for (int i = 0; i < 3; ++i) {
        for (int j = (i % 2 == 0 ? 0 : 1); j < BOARD_SIZE; j += 2) {
            board[i][j] = Piece::BLACK;
        }
    }

    for (int i = BOARD_SIZE - 3; i < BOARD_SIZE; ++i) {
        for (int j = (i % 2 == 0 ? 0 : 1); j < BOARD_SIZE; j += 2) {
            board[i][j] = Piece::RED;
        }
    }
}

bool Checkers::isValidMove(int startRow, int startCol, int endRow, int endCol) const {
    if (startRow < 0 || startRow >= BOARD_SIZE || startCol < 0 || startCol >= BOARD_SIZE ||
        endRow < 0 || endRow >= BOARD_SIZE || endCol < 0 || endCol >= BOARD_SIZE) {
        return false;
    }
    if (board[endRow][endCol] != Piece::EMPTY) {
        return false; // No movement
    }

    Piece piece = board[startRow][startCol];
    if (currentPlayer == Player::RED) {
        if (piece == Piece::RED && endRow > startRow && std::abs(endRow - startRow) == 1 && std::abs(endCol - startCol) == 1) {
            return true;
        }
        else if (piece == Piece::RED_KING && std::abs(endRow - startRow) == 1 && std::abs(endCol - startCol) == 1) {
            return true; // Regular move for white king
        }
    }
    else if (currentPlayer == Player::BLACK) {
        if (piece == Piece::BLACK && endRow < startRow && std::abs(endRow - startRow) == 1 && std::abs(endCol - startCol) == 1) {
            return true;
        }
        else if (piece == Piece::BLACK_KING && std::abs(endRow - startRow) == 1 && std::abs(endCol - startCol) == 1) {
            return true; // Regular move for black king
        }
    }
    return false;
}
bool Checkers::isJumpMove(int startRow, int startCol, int endRow, int endCol) const {
    if (startRow < 0 || startRow >= BOARD_SIZE || startCol < 0 || startCol >= BOARD_SIZE ||
        endRow < 0 || endRow >= BOARD_SIZE || endCol < 0 || endCol >= BOARD_SIZE) {
        return false;
    }
    if (board[endRow][endCol] != Piece::EMPTY) {
        return false; // No movement
    }
    if (std::abs(endRow - startRow) != 2 || std::abs(endCol - startCol) != 2) {
        return false;
    }

    int jumpRow = (startRow + endRow) / 2;
    int jumpCol = (startCol + endCol) / 2;
    Piece jumpPiece = board[jumpRow][jumpCol];

    if (currentPlayer == Player::RED) {
        return (board[startRow][startCol] == Piece::RED || board[startRow][startCol] == Piece::RED_KING) &&
            (jumpPiece == Piece::BLACK || jumpPiece == Piece::BLACK_KING);
    }
    else if (currentPlayer == Player::BLACK) {
        return (board[startRow][startCol] == Piece::BLACK || board[startRow][startCol] == Piece::BLACK_KING) &&
            (jumpPiece == Piece::RED || jumpPiece == Piece::RED_KING);
    }
	return false;
}

std::vector<std::pair<int, int>> Checkers::getPossibleJumps(int row, int col) const {
    std::vector<std::pair<int, int>> jumps;

    // Define jump directions
    const int dr[] = { -2, -2, 2, 2 }; // Row jumps
    const int dc[] = { -2, 2, -2, 2 }; // Column jumps

    for (int i = 0; i < 4; ++i) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (isJumpMove(row, col, newRow, newCol)) {
            jumps.push_back({ newRow, newCol });
        }
    }
    return jumps;
}

std::vector<std::pair<int, int>> Checkers::getPossibleMoves(int row, int col) const {
    std::vector<std::pair<int, int>> moves;
    Piece piece = board[row][col];
    int forwardDirection = 0;

	if (currentPlayer == Player::RED) {
		forwardDirection = 1; // White moves down
	}
	else if (currentPlayer == Player::BLACK) {
		forwardDirection = -1; // Black moves up
	}

    //move directions
    const int dr[] = { -1, -1, 1, 1 }; // Define row movement directions
    const int dc[] = { -1, 1, -1, 1 }; // Define column movement directions

    for (int i = 0; i < 4; ++i) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (currentPlayer == Player::RED) {
            if ((piece == Piece::RED && newRow > row) || piece == Piece::RED_KING) {
                if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && board[newRow][newCol] == Piece::EMPTY) {
                    moves.push_back({ newRow, newCol });
                }
            }
        }
    }
    return moves;
}

bool Checkers::canForceJump(Player player) const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Piece piece = board[i][j];
            if ((player == Player::RED && (piece == Piece::RED || piece == Piece::RED_KING)) ||
                (player == Player::BLACK && (piece == Piece::BLACK || piece == Piece::BLACK_KING))) {
                if (!getPossibleJumps(i, j).empty()) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Checkers::makeMove(int startRow, int startCol, int endRow, int endCol) {
    board[endRow][endCol] = board[startRow][startCol];
    board[startRow][startCol] = Piece::EMPTY;
    currentPlayer = (currentPlayer == Player::RED) ? Player::BLACK : Player::RED;
}

void Checkers::makeJump(int startRow, int startCol, int endRow, int endCol) {
    int jumpRow = (startRow + endRow) / 2;
    int jumpCol = (startCol + endCol) / 2;
    board[endRow][endCol] = board[startRow][startCol];
    board[startRow][startCol] = Piece::EMPTY;
    board[jumpRow][jumpCol] = Piece::EMPTY; // Remove the jumped piece

    if (!getPossibleJumps(endRow, endCol).empty()) {
        // If the player can jump again, they must do so
        return;
    }
    currentPlayer = (currentPlayer == Player::RED) ? Player::BLACK : Player::RED;
}

void Checkers::updateKings() {
    for (int j = 0; j < BOARD_SIZE; ++j) {
        if (board[0][j] == Piece::BLACK) {
            board[0][j] = Piece::BLACK_KING;
        }
        if (board[BOARD_SIZE - 1][j] == Piece::RED) {
            board[BOARD_SIZE - 1][j] = Piece::RED_KING;
        }
    }
}

void Checkers::updateStatus() {
    bool whiteExist = false;
    bool blackExist = false;
    for (const auto& row : board) {
        for (const auto& piece : row) {
            if (piece == Piece::RED || piece == Piece::RED_KING) {
                whiteExist = true;
            }
            else if (piece == Piece::BLACK || piece == Piece::BLACK_KING) {
                blackExist = true;
            }
        }
    }
    if (!whiteExist) {
        gameStatus = Status::BLACK_WINS;
    }
    else if (!blackExist) {
        gameStatus = Status::RED_WINS;
    }
    else if (!canForceJump(currentPlayer)) {
        gameStatus = Status::ONGOING;
    }
}

Checkers::Piece Checkers::getPieceAt(int row, int col) const {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        return board[row][col];
    }
    return Piece::EMPTY; // Return empty if out of bounds
}

void Checkers::setPieceAt(int row, int col, Piece piece) {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        board[row][col] = piece;
    }
    else {
        std::cerr << "Error: Attempt to set piece out of bounds (" << row << ", " << col << ")" << std::endl;
    }
}

Checkers::Status Checkers::status() const {
    return gameStatus;
}

bool Checkers::play(int startRow, int startCol, int endRow, int endCol) {
    if (gameStatus != Status::ONGOING) {
        return false; // Game is already over
    }

    if (canForceJump(currentPlayer)) {
        if (isJumpMove(startRow, startCol, endRow, endCol)) {
            makeJump(startRow, startCol, endRow, endCol);
            updateKings();
            if (!getPossibleJumps(endRow, endCol).empty()) {
                std::cout << "You must make another jump move!" << std::endl;
                return true;
            }
            else {
                currentPlayer = (currentPlayer == Player::RED) ? Player::BLACK : Player::RED;
                updateStatus();
                return true; // move successful
            }
        } else {
            std::cout << "You must make a jump move!" << std::endl;
            return false; // move failed
        }
    } else {
        if (isValidMove(startRow, startCol, endRow, endCol)) {
            makeMove(startRow, startCol, endRow, endCol);
            updateKings();
            updateStatus();
            return true; // move successful
        }
        else if (isJumpMove(startRow, startCol, endRow, endCol)) {
            makeJump(startRow, startCol, endRow, endCol);
            updateKings();
            updateStatus();
            return true; // move successful
        }
        else {
            std::cout << "Invalid move!" << std::endl;
            return false; // move failed
        }
    }
}

void Checkers::display() const {
    std::cout << " ";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << " " << i;
    }
    std::cout << std::endl;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << i << " ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            std::cout << static_cast<char>(board[i][j]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Current Player: " << (currentPlayer == Player::RED ? "White" : "Black") << std::endl;
    if (gameStatus != Status::ONGOING) {
        std::cout << "GAME OVER: ";
        if (gameStatus == Status::RED_WINS) {
            std::cout << "White wins!" << std::endl;
        }
        else if (gameStatus == Status::BLACK_WINS) {
            std::cout << "Black wins!" << std::endl;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Checkers& game) {
    os << " ";
    for (int i = 0; i < 8; ++i) {
        os << i << " ";
    }
    os << std::endl;
    for (int i = 0; i < 8; ++i) {
        os << i << " ";
        for (int j = 0; j < 8; ++j) {
            os << static_cast<char>(game.board[i][j]) << " ";
        }
        os << std::endl;
    }
    os << "Current Player: " << (game.currentPlayer == Checkers::Player::RED ? "White" : "Black") << std::endl;
    if (game.gameStatus != Checkers::Status::ONGOING) {
        os << "GAME OVER: ";
        if (game.gameStatus == Checkers::Status::RED_WINS) {
            os << "White wins!" << std::endl;
        }
        else if (game.gameStatus == Checkers::Status::BLACK_WINS) {
            os << "Black wins!" << std::endl;
        }
    }
    return os;
}
Checkers::Player Checkers::getCurrentPlayer() const {
	return currentPlayer;
}