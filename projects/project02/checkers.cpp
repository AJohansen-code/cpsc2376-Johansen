#pragma once

#include "checkers.h"
#include <iostream>
#include <vector>
#include <cmath>

const int BOARD_SIZE = 8; // Define the board size

Checkers::Checkers() : currentPlayer(Player::WHITE), gameStatus(Status::ONGOING) {
    board.resize(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE, Piece::EMPTY));

    for (int i = 0; i < 3; ++i) {
        for (int j = (i % 2 == 0 ? 0 : 1); j < BOARD_SIZE; j += 2) {
            board[i][j] = Piece::BLACK;
        }
    }

    for (int i = BOARD_SIZE - 3; i < BOARD_SIZE; ++i) {
        for (int j = (i % 2 == 0 ? 0 : 1); j < BOARD_SIZE; j += 2) {
            board[i][j] = Piece::WHITE;
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
    if (currentPlayer == Player::WHITE) {
        if (piece == Piece::WHITE && endRow > startRow && std::abs(endRow - startRow) == 1 && std::abs(endCol - startCol) == 1) {
            return true;
        }
        else if (piece == Piece::WHITE_KING && std::abs(endRow - startRow) == 1 && std::abs(endCol - startCol) == 1) {
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

    if (currentPlayer == Player::WHITE) {
        return (board[startRow][startCol] == Piece::WHITE || board[startRow][startCol] == Piece::WHITE_KING) &&
            (jumpPiece == Piece::BLACK || jumpPiece == Piece::BLACK_KING);
    }
    else if (currentPlayer == Player::BLACK) {
        return (board[startRow][startCol] == Piece::BLACK || board[startRow][startCol] == Piece::BLACK_KING) &&
            (jumpPiece == Piece::WHITE || jumpPiece == Piece::WHITE_KING);
    }
}

std::vector<std::pair<int, int>> Checkers::getPossibleJumps(int row, int col) const {
    std::vector<std::pair<int, int>> jumps;
    Piece piece = board[row][col];

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

    //move directions
    const int dr[] = { -1, -1, 1, 1 }; // Row moves
    const int dc[] = { -1, 1, -1, 1 }; // Column moves

    for (int i = 0; i < 4; ++i) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (currentPlayer == Player::WHITE) {
            if ((piece == Piece::WHITE && newRow > row) || piece == Piece::WHITE_KING) {
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
            if ((player == Player::WHITE && (piece == Piece::WHITE || piece == Piece::WHITE_KING)) ||
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
    currentPlayer = (currentPlayer == Player::WHITE) ? Player::BLACK : Player::WHITE;
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
    currentPlayer = (currentPlayer == Player::WHITE) ? Player::BLACK : Player::WHITE;
}

void Checkers::updateKings() {
    for (int j = 0; j < BOARD_SIZE; ++j) {
        if (board[0][j] == Piece::BLACK) {
            board[0][j] = Piece::BLACK_KING;
        }
        if (board[BOARD_SIZE - 1][j] == Piece::WHITE) {
            board[BOARD_SIZE - 1][j] = Piece::WHITE_KING;
        }
    }
}

void Checkers::updateStatus() {
    bool whiteExist = false;
    bool blackExist = false;
    for (const auto& row : board) {
        for (const auto& piece : row) {
            if (piece == Piece::WHITE || piece == Piece::WHITE_KING) {
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
        gameStatus = Status::WHITE_WINS;
    }
    else if (!canForceJump(currentPlayer)) {
        gameStatus = Status::ONGOING;
    }
}

Checkers::Status Checkers::status() const {
    return gameStatus;
}

void Checkers::play(int startRow, int startCol, int endRow, int endCol) {
    if (gameStatus != Status::ONGOING) {
        return;
    }

    if (canForceJump(currentPlayer)) {
        if (isJumpMove(startRow, startCol, endRow, endCol)) {
            makeJump(startRow, startCol, endRow, endCol);
            updateKings();
            updateStatus();
        }
    }
    else {
        if (isValidMove(startRow, startCol, endRow, endCol)) {
            makeMove(startRow, startCol, endRow, endCol);
            updateKings();
            updateStatus();
        }
        else if (isJumpMove(startRow, startCol, endRow, endCol)) {
            makeJump(startRow, startCol, endRow, endCol);
            updateKings();
            updateStatus();
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
    std::cout << "Current Player: " << (currentPlayer == Player::WHITE ? "White" : "Black") << std::endl;
    if (gameStatus != Status::ONGOING) {
        std::cout << "GAME OVER: ";
        if (gameStatus == Status::WHITE_WINS) {
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
    os << "Current Player: " << (game.currentPlayer == Checkers::Player::WHITE ? "White" : "Black") << std::endl;
    if (game.gameStatus != Checkers::Status::ONGOING) {
        os << "GAME OVER: ";
        if (game.gameStatus == Checkers::Status::WHITE_WINS) {
            os << "White wins!" << std::endl;
        }
        else if (game.gameStatus == Checkers::Status::BLACK_WINS) {
            os << "Black wins!" << std::endl;
        }
    }
    return os;
}
