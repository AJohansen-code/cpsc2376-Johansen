
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <limits>

#include "checkers.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int SQUARE_SIZE = 60;

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;	
}


	SDL_Window* window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	Checkers game;
	bool quit = false;
	SDL_Event event;

	int selectedRow = -1;
	int selectedCol = -1;
	bool selectingStart = true;
	int startRow = -1;
	int startCol = -1;

	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;
	SDL_Color textColor = { 255, 255, 255, 255 }; // White text

	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					if (selectedRow > 0) selectedRow--;
					else if (selectedRow == -1) selectedRow = 0;
					break;
				case SDLK_DOWN:
					if (selectedRow < 7) selectedRow++;
					else if (selectedRow == -1) selectedRow = 0;
					break;
				case SDLK_LEFT:
					if (selectedCol > 0) selectedCol--;
					else if (selectedCol == -1) selectedCol = 0;
					break;
				case SDLK_RIGHT:
					if (selectedCol < 7) selectedCol++;
					else if (selectedCol == -1) selectedCol = 0;
					break;
				case SDLK_RETURN:// enter key
					if (game.status() == Checkers::Status::ONGOING) { // Only allow moves if the game is ongoing
						if (selectedRow != -1 && selectedCol != -1) {
							if (selectingStart) {
								Checkers::Piece piece = game.getPieceAt(selectedRow, selectedCol);
								Checkers::Player currentPlayer = game.getCurrentPlayer();
								if ((currentPlayer == Checkers::Player::RED && (piece == Checkers::Piece::RED || piece == Checkers::Piece::RED_KING)) ||
									(currentPlayer == Checkers::Player::BLACK && (piece == Checkers::Piece::BLACK || piece == Checkers::Piece::BLACK_KING))) {
									startRow = selectedRow;
									startCol = selectedCol;
									selectingStart = false;
								}
								else {
									std::cout << "Invalid selection! Select your piece." << std::endl;
								}
							} else {
								if (game.play(startRow, startCol, selectedRow, selectedCol)) {
									if (game.canForceJump(game.getCurrentPlayer()) && game.getPossibleJumps(selectedRow, selectedCol).empty()) {
										selectingStart = true;
										selectedRow = selectedCol = -1;
									}
									else if (!game.canForceJump(game.getCurrentPlayer())) {
										selectingStart = true;
										selectedRow = selectedCol = -1;
									}
								}
								else {
									std::cout << "Invalid move! Select a valid destination." << std::endl;
								}
							}
						}
					} else {
						std::cout << "Game over. Press 'R' to restart." << std::endl;
					}
					break;
                           
				case SDLK_r:// Restart Key
					game = Checkers();
					selectedRow = selectedCol = -1;
					selectingStart = true;
					break;
				case SDLK_ESCAPE:// Escape Key
					quit = true;
					break;
				default:
					break;
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
		SDL_RenderClear(renderer);
		
		game.draw(renderer, selectedRow, selectedCol, { startRow, startCol });

		if (game.status() != Checkers::Status::ONGOING) {
			std::string message;

			if (game.status() == Checkers::Status::RED_WINS) {
				message = "Red wins! Press 'R' to restart.";
			}
			else if (game.status() == Checkers::Status::BLACK_WINS) {
				message = "Black wins! Press 'R' to restart.";
			}
			else if (game.status() == Checkers::Status::DRAW) {
				message = "Draw! Press 'R' to restart.";
			}

			textSurface = SDL_CreateRGBSurfaceWithFormat(0, 300, 50, 32, SDL_PIXELFORMAT_RGBA32);
			if (textSurface == nullptr) {
				std::cerr << "SDL_CreateRGBSurfaceWithFormat failed: " << SDL_GetError() << std::endl;
			}
			else {
				SDL_FillRect(textSurface, NULL, SDL_MapRGBA(textSurface->format, 0, 0, 0, 0)); // Transparent background

				
					std::cout << message << std::endl;
			}
		}

		SDL_RenderPresent(renderer);

		SDL_Delay(50);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

