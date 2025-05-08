#pragma once 

#include <vector>  
#include <iostream>  
#include <utility> // for std::pair 
#include <SDL2/SDL.h>
#include "Engine.h"

class Checkers {  
public:  
  enum class Piece : char {  
      EMPTY = '.',  
      RED = 'r',  
      BLACK = 'b',  
      RED_KING = 'R',  
      BLACK_KING = 'B'  
  };  

  enum class Player {  
      RED,  
      BLACK  
  };  

  enum class Status {  
      ONGOING,  
      RED_WINS,  
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
  void makeMove(int startRow, int startCol, int endRow, int endCol);  
  void makeJump(int startRow, int startCol, int endRow, int endCol);  
  void updateKings();  
  void updateStatus();   
  void setPieceAt(int row, int col, Piece piece);  

public:  
  Piece getPieceAt(int row, int col) const;  
  void draw(SDL_Renderer* renderer, int selectedRow, int selectedCol, std::pair<int, int> startPos);
  Checkers();
  bool play(int startRow, int startCol, int endRow, int endCol);  
  Status status() const;  
  void display() const; 
  Player getCurrentPlayer() const;

  bool canForceJump(Player player) const;

  std::vector<std::pair<int, int>> getPossibleJumps(int row, int col) const;
  std::vector<std::pair<int, int>> getPossibleMoves(int row, int col) const;

  friend std::ostream& operator<<(std::ostream& os, const Checkers& game);

};
