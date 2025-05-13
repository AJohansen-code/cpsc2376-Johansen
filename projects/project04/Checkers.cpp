#include "Checkers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <iostream>
#include <vector>
#include <cmath>

Checkers::Checkers() :
	board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE, Piece::EMPTY)),
	currentPlayer(Player::BLACK),
	selectedPiece({ -1, -1 }),
	inJumpSequence(false),
	jumpingPiece({ -1, -1 })
{

}



void Checkers::initializeBoard() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if ((i + j) % 2 == 1) {
				if (i < 3) {
					board[i][j] = Piece::RED;
				}
				else if (i > 4) {
					board[i][j] = Piece::BLACK;
			}

				else {
					board[i][j] = Piece::EMPTY;
				}
			}
			else {
				board[i][j] = Piece::EMPTY;
			}
		}
	}

	currentPlayer = Player::BLACK; // Reset the current player to Black
	selectedPiece = { -1, -1 };
	inJumpSequence = false;
	jumpingPiece = { -1, -1 };
}

void Checkers::draw(SDL_Renderer* renderer, int squareSize) const {
	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			SDL_Rect square = { col * squareSize, row * squareSize, squareSize, squareSize };

			if ((row + col) % 2 == 0) {
				SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 222, 184, 135, 255);
			}
			SDL_RenderFillRect(renderer, &square);

			if (selectedPiece.x == col && selectedPiece.y == row) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 150);
				SDL_RenderFillRect(renderer, &square);
			}
			// Highlight potential move squares
			std::vector<Point> possibleMoves = getPotentialMoves(row, col, board[row][col], false);

			for (const auto& move : possibleMoves) {
				if (move.x == col && move.y == row) {
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 150);
					SDL_RenderFillRect(renderer, &square);
				}
			}

		}

	}

	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			drawPiece(renderer, board[row][col], col * squareSize + squareSize / 2, row * squareSize + squareSize / 2, squareSize / 3);
		}
	}
}

void Checkers::handleInput(const SDL_Event& event, int squareSize) {
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		int mouseX = event.button.x;
		int mouseY = event.button.y;
		int clickedCol = mouseX / squareSize;
		int clickedRow = mouseY / squareSize;

		if (isWithinBounds(clickedRow, clickedCol)) {
			std::cout << "Clicked on square: (" << clickedRow << ", " << clickedCol << ")" << std::endl;

			if (selectedPiece.x != -1) {
				bool validMove = false;
				std::vector<Point> possibleMoves = getPotentialMoves(selectedPiece.y, selectedPiece.x, board[selectedPiece.y][selectedPiece.x], false);
				for (const auto& move : possibleMoves) {
					if (move.x == clickedCol && move.y == clickedRow) {
						// Perform the move
						board[clickedRow][clickedCol] = board[selectedPiece.y][selectedPiece.x];
						board[selectedPiece.y][selectedPiece.x] = Piece::EMPTY;

						bool jumped = false;
						if (std::abs(clickedRow - selectedPiece.y) == 2) {
							int jumpedRow = (clickedRow + selectedPiece.y) / 2;
							int jumpedCol = (clickedCol + selectedPiece.x) / 2;
							board[jumpedRow][jumpedCol] = Piece::EMPTY;
							std::cout << "Jumped over a piece." << std::endl;
							jumped = true;
							jumpingPiece = { clickedCol, clickedRow };
						}

						std::vector<Point> furtherJumps = getPotentialMoves(clickedRow, clickedCol, board[clickedRow][clickedCol], true);
						if (jumped && !furtherJumps.empty())

						{
							inJumpSequence = true;
							possibleMoves = furtherJumps;
							selectedPiece = { clickedCol, clickedRow };
							std::cout << "Possible follow-up jumps available" << std::endl;
						}
						else
						{
							selectedPiece = { -1, -1 };
							inJumpSequence = false;
							jumpingPiece = { -1, -1 };
							currentPlayer = (currentPlayer == Player::BLACK) ? Player::RED : Player::BLACK;
							std::cout << "Move made. Current Player: " << currentPlayer << std::endl;
						}
						validMove = true;
						break;
					}
				}

				if (!validMove) {
					selectedPiece = { -1, -1 };
					inJumpSequence = false;
					jumpingPiece = { -1, -1 };
					std::cout << "Invalid move. Deselected." << std::endl;
				}
			}
			else {
				if (board[clickedRow][clickedCol] != Piece::EMPTY &&
					((currentPlayer == Player::BLACK && (board[clickedRow][clickedCol] == Piece::BLACK || board[clickedRow][clickedCol] == Piece::BLACK_KING)) ||
						(currentPlayer == Player::RED && (board[clickedRow][clickedCol] == Piece::RED || board[clickedRow][clickedCol] == Piece::RED_KING)))) {

					selectedPiece = { clickedCol, clickedRow };
					std::vector<Point> possibleMoves = getPotentialMoves(clickedRow, clickedCol, board[clickedRow][clickedCol], hasForcedJump(currentPlayer));
					if (hasForcedJump(currentPlayer))
					{
						std::vector<Point> onlyJumps;
						for (const auto& move : possibleMoves)
						{
							if (std::abs(move.y - clickedRow) == 2)
							{
								onlyJumps.push_back(move);
							}
						}
						possibleMoves = onlyJumps;
					}

					std::cout << "Selected piece at: (" << selectedPiece.y << ", " << selectedPiece.x << "), type: " << static_cast<int>(board[clickedRow][clickedCol]) << std::endl;
					std::cout << "Possible moves: " << possibleMoves.size() << std::endl;
					for (const auto& move : possibleMoves) {
						std::cout << "(" << move.y << ", " << move.x << ") ";
					}
					std::cout << std::endl;
				}
				else {
					std::cout << "Cannot select opponent's piece or an empty square." << std::endl;
				}
			}
		}
	}
}

std::vector<Point> Checkers::getPotentialMoves(int row, int col, Piece piece, bool onlyJumps) const {
	std::vector<Point> potentialMoves;
	int pieceColor = (piece == Piece::BLACK || piece == Piece::BLACK_KING) ? 1 : 2;
	int opponentColor = (pieceColor == 1) ? 2 : 1;
	bool isKing = (piece == Piece::BLACK_KING || piece == Piece::RED_KING);

	int directions[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

	for (const auto& dir : directions) {
		int dr = dir[0];
		int dc = dir[1];

		if (!onlyJumps) {
			int nextRow = row + dr;
			int nextCol = col + dc;

			if (isWithinBounds(nextRow, nextCol) && board[nextRow][nextCol] == Piece::EMPTY && (row + col) % 2 == 1) {
				if (!isKing && dr != (pieceColor == 1 ? -1 : 1)) {
					continue;
				}
				potentialMoves.push_back({ nextCol, nextRow });
			}
		}

		int jumpOverRow = row + dr;
		int jumpOverCol = col + dc;
		int landRow = row + 2 * dr;
		int landCol = col + 2 * dc;

		if (isWithinBounds(jumpOverRow, jumpOverCol) &&
			isWithinBounds(landRow, landCol) &&
			board[landRow][landCol] == Piece::EMPTY) {
			int jumpedPiece = static_cast<int>(board[jumpOverRow][jumpOverCol]);
			bool isOpponent = (pieceColor == 1 && (jumpedPiece == 2 || jumpedPiece == 4)) ||

				(pieceColor == 2 && (jumpedPiece == 1 || jumpedPiece == 3));

			if (isOpponent) {
				if (!isKing && dr != (pieceColor == 1 ? -1 : 1)) {
					continue;
				}
				potentialMoves.push_back({ landCol, landRow });
			}
		}
	}
	return potentialMoves;
}



bool Checkers::hasForcedJump(Player player) const {
	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			Piece piece = board[row][col];
			if ((player == Player::BLACK && (piece == Piece::BLACK || piece == Piece::BLACK_KING)) ||
				(player == Player::RED && (piece == Piece::RED || piece == Piece::RED_KING))) {
				std::vector<Point> jumps = getPotentialMoves(row, col, piece, true);
				if (!jumps.empty()) {
					return true;
				}
			}
		}
	}
	return false;
}


Checkers::Piece Checkers::getPieceAt(int row, int col) const {
	if (isWithinBounds(row, col)) {
		return board[row][col];
	}
	return Piece::EMPTY;
}

void Checkers::setPieceAt(int row, int col, Piece piece) {
	if (isWithinBounds(row, col)) {
		board[row][col] = piece;
	}
}

bool Checkers::isWithinBounds(int row, int col) const {
	return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE);
}

void Checkers::drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) const {
	for (int w = -radius; w <= radius; ++w) {
		for (int h = -radius; h <= radius; ++h) {
			if (w * w + h * h <= radius * radius) {
				SDL_RenderDrawPoint(renderer, centerX + w, centerY + h);
			}
		}
	}
}

void Checkers::drawPiece(SDL_Renderer* renderer, Piece piece, int centerX, int centerY, int radius) const {
	SDL_Color color;
	switch (piece) {
	case Piece::BLACK:
	case Piece::BLACK_KING:
		color = { 0, 0, 0, 255 };
		break;
	case Piece::RED:
	case Piece::RED_KING:
		color = { 255, 0, 0, 255 };
		break;
	default:
		return;
	}

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	drawCircle(renderer, centerX, centerY, radius);

	if (piece == Piece::BLACK_KING || piece == Piece::RED_KING) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		drawCircle(renderer, centerX, centerY, radius / 2);
	}
}

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	int screenWidth;
	int screenHeight;
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	int squareSize = std::min(screenWidth / Checkers::BOARD_SIZE, screenHeight / Checkers::BOARD_SIZE);

	Checkers game;

	game.initializeBoard();

	SDL_Event event;
	bool quit = false;
	std::vector<Point> possibleMoves;
	bool inJumpSequence = false;
	Point jumpingPiece = { -1, -1 };

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}

			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_r:
					std::cout << "Restarting the game." << std::endl;
					game.initializeBoard();
					possibleMoves.clear(); // Reset possible moves
					inJumpSequence = false; // Reset jump sequence flag
					jumpingPiece = { -1, -1 }; // Reset jumping piece
					break;

				case SDLK_ESCAPE:
					std::cout << "Quitting the game." << std::endl;
					quit = true;
					break;
				default:
					break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				game.handleInput(event, squareSize);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		game.draw(renderer, squareSize);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}