#include "checkers.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>

const int BOARD_SIZE = 8;
const int SQUARE_SIZE = 60;

void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
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
    if (!isWithinBounds(startRow,startCol) || !isWithinBounds(endRow, endCol) ||
        board[endRow][endCol] != Piece::EMPTY) {
        return false;
    }
	int deltaRow = endRow - startRow;
	int deltaCol = std::abs(endCol - startCol);
	Piece piece = board[startRow][startCol];

	if (deltaCol != 1 || std::abs(deltaRow) != 1) {
		std::cout << "Invalid move: Not a diagonal move." << std::endl;
		return false; // Invalid move
	}

    if (currentPlayer == Player::RED) {
        if (piece == Piece::RED && deltaRow > 0) {
            return true;
        }
        else if (piece == Piece::RED_KING) {
            return true;
        }
        else {
            std::cout << "Invalid move: Regular move for red piece." << std::endl;
        }
    } else if (currentPlayer == Player::BLACK) {
        if (piece == Piece::BLACK && deltaRow < 0) {
            return true;
        }
        else if (piece == Piece::BLACK_KING) {
            return true; // Regular move for black king
		}
		else {
			std::cout << "Invalid move: Regular move for black piece." << std::endl;
         }
  
    }
    return false;
}

bool Checkers::isWithinBounds(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

bool Checkers::isJumpMove(int startRow, int startCol, int endRow, int endCol) const {
	std::cout << "isJumpMove called: start= (" << startRow << ", " << startCol << ") end=(" << endRow << "," << endCol << ") player=" << (currentPlayer == Player::RED ? "Red" : "Black") << " piece=" << static_cast<char>(board[startRow][startCol]) << std::endl;
    if (!isWithinBounds(startRow, startCol) || !isWithinBounds(endRow, endCol) || board[endRow][endCol] != Piece::EMPTY) {
        return false;
    }
    if (std::abs(endRow - startRow) != 2 || std::abs(endCol - startCol) != 2) {
        return false;
    }

    int jumpRow = (startRow + endRow) / 2;
    int jumpCol = (startCol + endCol) / 2;
    Piece jumpedPiece = board[jumpRow][jumpCol];
	Piece piece = board[startRow][startCol];

    if (jumpedPiece == Piece::EMPTY) return false;

    if (currentPlayer == Player::RED) {
        if (piece == Piece::RED && endRow > startRow || piece == Piece::RED_KING) {
            return (jumpedPiece == Piece::BLACK || jumpedPiece == Piece::BLACK_KING);
        }
    }
    else if (currentPlayer == Player::BLACK) {
        if (piece == Piece::BLACK && endRow < startRow || piece == Piece::BLACK_KING) {
            return (jumpedPiece == Piece::RED || jumpedPiece == Piece::RED_KING);
        }
        else if (piece == Piece::BLACK_KING) {
            return (jumpedPiece == Piece::RED || jumpedPiece == Piece::RED_KING);
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
    bool redExists = false;
    bool blackExists = false;
	bool redCanMove = false;
	bool blackCanMove = false;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Piece piece = board[i][j];
            if (piece == Piece::RED || piece == Piece::RED_KING) {
                redExists = true;
                if (currentPlayer == Player::RED && (!getPossibleMoves(i, j).empty() || !getPossibleJumps(i, j).empty())) {
                    redCanMove = true;
                }
            }
            else if (piece == Piece::BLACK || piece == Piece::BLACK_KING) {
                blackExists = true;
                if (currentPlayer == Player::BLACK && (!getPossibleMoves(i, j).empty() || !getPossibleJumps(i, j).empty())) {
                    blackCanMove = true;
                }
            }
        }
    }

    if (!redExists) {
        gameStatus = Status::BLACK_WINS;
    }
    else if (!blackExists) {
        gameStatus = Status::RED_WINS;
    }
    else if (canForceJump(currentPlayer)) {
        gameStatus = Status::ONGOING;
    }
    else if (currentPlayer == Player::RED && !redCanMove) {
        gameStatus = Status::BLACK_WINS; // Or DRAW depending on rules
    }
    else if (currentPlayer == Player::BLACK && !blackCanMove) {
        gameStatus = Status::RED_WINS; // Or DRAW depending on rules
    }
    else {
        gameStatus = Status::ONGOING;
    }
}

void Checkers::setPieceAt(int row, int col, Piece piece) {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        board[row][col] = piece;
    }
    else {
        std::cerr << "Error: Attempt to set piece out of bounds (" << row << ", " << col << ")" << std::endl;
    }
}
Checkers::Piece Checkers::getPieceAt(int row, int col) const {
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        return board[row][col];
    }
    return Piece::EMPTY; // Return empty if out of bounds
}

Checkers::Status Checkers::status() const {
    return gameStatus;
}

bool Checkers::play(int startRow, int startCol, int endRow, int endCol) {
	std::cout << "Playing move from (" << startRow << ", " << startCol << ") to (" << endRow << ", " << endCol << ")" << std::endl;
    if (gameStatus != Status::ONGOING) {
		std::cout << "Game is already over!" << std::endl;
        return false; // Game is already over
    }

    if (canForceJump(currentPlayer)) {
		std::cout << "Forced jump is available." << std::endl;
        if (isJumpMove(startRow, startCol, endRow, endCol)) {
			std::cout << "Jump move detected." << std::endl;
            makeJump(startRow, startCol, endRow, endCol);
            updateKings();
            if (!getPossibleJumps(endRow, endCol).empty()) {
                std::cout << "You must make another jump move!" << std::endl;
                return true;
            }
            else {
                currentPlayer = (currentPlayer == Player::RED) ? Player::BLACK : Player::RED;
                updateStatus();
				std::cout << "No more jumps available. Switching player." << std::endl;
                return true; // move successful
            }
        }
        else {
            std::cout << "Invalid move: You must make a jump move!" << std::endl;
            return false; // move failed
        }
    }
    else {
		std::cout << "No forced jump available." << std::endl;
        if (isValidMove(startRow, startCol, endRow, endCol)) {
			std::cout << "Valid move detected." << std::endl;
            makeMove(startRow, startCol, endRow, endCol);
            updateKings();
            updateStatus();
			std::cout << "Regular move successful, switch to next player: " << (currentPlayer == Player::RED ? "Red" : "Black") << std::endl;
            return true; // move successful
        }
        else if (isJumpMove(startRow, startCol, endRow, endCol)) {
			std::cout << "Jump move detected." << std::endl;
            makeJump(startRow, startCol, endRow, endCol);
            updateKings();
            updateStatus();
			std::cout << "Jump move successful, switch to next player: " << (currentPlayer == Player::RED ? "Red" : "Black") << std::endl;
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

    const int dr[] = { -1, -1, 1, 1 }; // Define row movement directions
    const int dc[] = { -1, 1, -1, 1 }; // Define column movement directions

    for (int i = 0; i < 4; ++i) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];

		if (isWithinBounds(newRow, newCol) && board[newRow][newCol] == Piece::EMPTY) {
            if (currentPlayer == Player::RED) {
                if ((piece == Piece::RED && newRow > row) || piece == Piece::RED_KING) {
                    moves.push_back({ newRow, newCol });
                }
			}
			else if (currentPlayer == Player::BLACK) {
				if ((piece == Piece::BLACK && newRow < row) || piece == Piece::BLACK_KING) {
					moves.push_back({ newRow, newCol });
				}
			}
        }
    }
    return moves;
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
