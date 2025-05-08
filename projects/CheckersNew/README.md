# Project 04 – Interactive Checkers

## 🕹️ Description

This is an interactive, turn-based Checkers game implemented in C++ using the SDL2 library for graphics rendering. 
Players take turns moving their pieces across the board, aiming to capture all of their opponent's pieces. 
The game features keyboard controls for selecting and moving pieces, clear visual feedback, and the ability to replay after a game concludes. 
This project builds upon the foundation of Project 02, extending the `Game` class with interactive elements and game lifecycle management.

## 🎮 Controls

- **Navigation:** Use the Arrow Keys (↑, ↓, ←, →) to navigate the game board and select a piece.
- **Selection/Move:** Press the Enter key to select a piece you want to move or to confirm a valid move to an empty square.
- **Restart:** Press the `R` key to start a new game after the current game ends.
- **Exit:** Press the Escape key (`Esc`) to quit the game.

## 🧪 Screenshot

![Screenshot](screenshot.png)

## ✨ Extra Features

- **Standard Checkers Rules:** Implements the fundamental rules of Checkers, including regular moves and mandatory jumps.
- **Kinging:** Pieces that reach the opposite side of the board are promoted to Kings.
- **Visual Move Highlighting:** When a piece is selected, valid moves are highlighted on the board.
- **Turn Indicator:** Clearly displays whose turn it is.
- **Win/Loss Display:** Upon the conclusion of the game, a clear message indicates the winner or a draw (if applicable).
- **Keyboard-Centric Gameplay:** Designed for comfortable and intuitive gameplay using only the keyboard.

## 🛠️ Engine

This project may utilize and/or modify the engine provided in the following repository:

[https://github.com/NicholasSeward/cpsc2376-sewardn/tree/main/projects/Project04](https://github.com/NicholasSeward/cpsc2376-sewardn/tree/main/projects/Project04)

The `Game` class within this project encapsulates all the core game logic, adhering to the specifications of Project 02. 
Input handling and SDL2 rendering are implemented outside of this core `Game` class to maintain a clean, object-oriented structure.

## 📁 File Structurezl
