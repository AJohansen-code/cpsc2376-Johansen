#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <algorithm>

enum class Player {none, player1, player2};
enum class GameState {running, draw, player1_win, player2_win};
enum class MoveType {Move, Wall};
enum class Direction {Up, Down, Left, Right, horizontal, vertical};

struct Wall {
    int x;
    int y;
    Direction direction;
};

const int Board_Size = 9;
const int Wall_Count = 10;

void displayRules();
std::vector<std::vector<Player>> makeBoard(int& player1walls, int& player2walls, int& player1Row, int& player2Row);

void displayBoard(const std::vector<std::vector<Player>>& board, int player1walls, int player2walls, const std::vector<Wall>& placedWalls);
GameState gameStatus(const std::vector<std::vector<Player>>& board, int player1Row, int player2Row);

bool isValidMove(const std::vector<std::vector<Player>>& board, int currentRow, int currentCol, int newRow, int newCol, const std::vector<Wall>& placedWalls, int player1Row, int player2Row, int player1col, int player2col, Player currentPlayer);
bool isValidWallPlacement(const std::vector<std::vector<Player>>& board, const Wall& wall, const std::vector<Wall>& placedWalls, int player1Row, int player2Row, int player1walls, int player2walls);
bool canReachGoal(const std::vector<std::vector<Player>>& board, int startRow, int startCol, int goalRow, const std::vector<Wall>& placedWalls);

void play(std::vector<std::vector<Player>>& board, int& player1Row, int& player2Row, int& player1walls, int& player2walls, std::vector<Wall>& placedWalls);
bool getPlayerMove(Player currentPlayer, int& moveRow, int& moveCol, MoveType& moveType, Direction& wallDirection);

void clearInputBuffer();

bool isWallOverlap(const std::vector<Wall>& placedWalls, const Wall& newWall);
bool canJump(const std::vector<std::vector<Player>>& board, int currentRow, int currentCol, int newRow, int newCol, int player1Row, int player2Row, int player1Col, int player2Col);
bool bfs(const std::vector<std::vector<Player>>& board, int startRow, int startCol, int goalRow, int goalCol, const std::vector<Wall>& placedWalls);

void startGame();

int main() {
    startGame();
    return 0;
}

void startGame() {
    displayRules();
    char choice = 'y';
    while (choice == 'y' || choice == 'Y'){
    int player1walls = Wall_Count;
    int player2walls = Wall_Count;
    int player1Row = Board_Size - 1;
    int player2Row = 0;
    int player1Col = Board_Size / 2;
    int player2Col = Board_Size / 2;
    std::vector<std::vector<Player>> board = makeBoard(player1walls, player2walls, player1Row, player2Row);
    
    std::vector<Wall> walls;
    play(board, player1Row, player2Row, player1walls, player2walls, walls);

    std::cout << "Play Game Again? (y/n): ";
    std::cin >> choice;
    clearInputBuffer();
    }   
}

void displayRules() {
    std::cout << "Welcome to Quoridor!" << std::endl;
    std::cout << "Rules:" << std::endl;
    std::cout << "1. Each player starts at opposite ends of the board." << std::endl;
    std::cout << "2. Players can move one square orthogonally (up, down, left, right) per turn." << std::endl;
    std::cout << "3. Players can place a wall on the board to block their opponent's path." << std::endl;
    std::cout << "4. The first player to reach the opposite side wins." << std::endl;
    std::cout << "5. Each players has " << Wall_Count << " walls to place." << std::endl;
    std::cout << "6. Remeber to have fun!" << std::endl;
}

std::vector<std::vector<Player>> makeBoard(int& player1walls, int& player2walls, int& player1Row, int& player2Row) {
    std::vector<std::vector<Player>> board(Board_Size, std::vector<Player>(Board_Size, Player::none));
    board[player1Row][Board_Size / 2] = Player::player1;
    board[player2Row][Board_Size / 2] = Player::player2;
    return board;
}

void displayBoard(const std::vector<std::vector<Player>>& board, int player1walls, int player2walls, const std::vector<Wall>& placedWalls) {
    std::cout << "Player 1 Walls: " << player1walls << "Player 2 Walls: " << player2walls << std::endl;
    std::cout << "  ";
    for (int j = 0; j < Board_Size; ++j) {
        std::cout << j << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < Board_Size; ++i) {
        std::cout << i <<" ";
        for (int j = 0; j < Board_Size; ++j) {
            switch (board[i][j]) {
                    case Player::player1:std::cout << "1 ";
                    break;
                    case Player::player2:std::cout << "2 ";
                    break;
                    default:std::cout << ". ";
                        break;
                    }
                if (j < Board_Size - 1) {
                    bool verticalWallFound = false;
                    for (const auto& wall : placedWalls) {
                        if (wall.direction == Direction::vertical && wall.x == i && wall.y == j) {
                            std::cout << "|";
                            verticalWallFound = true;
                            break;
                        }
                    }
                    if (!verticalWallFound) std::cout << " ";
                }
            }
            std::cout << std::endl;
            if (i < Board_Size - 1 ) {
                std::cout << " ";
                for (int j = 0; j < Board_Size - 1; ++j) {
                    bool horizontalWallFound = false;
                    for (const auto& wall : placedWalls) {
                        if (wall.direction == Direction::horizontal && wall.x == i && wall.y == j) {
                            std::cout << "---";
                            horizontalWallFound = true;
                            break;
                        }
                    }
                    if (!horizontalWallFound) std::cout << " ";
                }
        std::cout << std::endl;
            }
        }
    }


GameState gameStatus(const std::vector<std::vector<Player>>& board, int player1Row, int player2Row) {
    if (player1Row == 0) {
        return GameState::player1_win;
    } 
    else if (player2Row == Board_Size - 1) {
        return GameState::player2_win;
    }
    return GameState::running;
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isValidMove(const std::vector<std::vector<Player>>& board, int currentRow, int currentCol, int newRow, int newCol, const std::vector<Wall>& placedWalls, int player1Row, int player2Row, int player1col, int player2col, Player currentPlayer) {
    if (newRow < 0 || newRow >= Board_Size || newCol < 0 || newCol >= Board_Size) {
        return false;
    }
    if (abs(currentRow - newRow) + abs(currentCol - newCol) > 1 && !canJump (board, currentRow, currentCol, newRow, newCol, player1Row, player2Row, player1col, player2col)) {
        return false;
    }
    if (canJump(board, currentRow, currentCol, newRow, newCol, player1Row, player2Row, player1col, player2col)) {
        return true;
    }
    if (board[newRow][newCol] != Player::none && !canJump(board, currentRow, currentCol, newRow, newCol, player1Row, player2Row, player1col, player2col)) {
        return false;
    }
    if (!canReachGoal(board, newRow, newCol, (currentPlayer == Player::player1 ? 0 : Board_Size - 1), placedWalls)) {
        return false;
    }
    return true;
}

bool isWallOverlap(const std::vector<Wall>& placedWalls, const Wall& newWall) {
    for (const auto& wall : placedWalls) {
        if (newWall.direction == wall.direction) {
            if (newWall.direction == Direction::horizontal && newWall.x == wall.x &&
                abs(newWall.y - wall.y) <= 1) {
                return true;
            } else if (newWall.direction == Direction::vertical && newWall.y == wall.y &&
                       std::max(newWall.x, wall.x) <= std::min(newWall.x + 1, wall.x + 1)) {
                return true;
            }
        }else{
            if(newWall.direction == Direction::horizontal && newWall.x == wall.x){
                if((newWall.y == wall.y || newWall.y == wall.y -1) && wall.x <= newWall.x && wall.x+1 >= newWall.x){
                    return true;
                }
                if((newWall.x == wall.x || newWall.x == wall.x -1) && wall.y <= newWall.y && wall.y+1 >= newWall.y){
                    return true;
                }
            }
        }
    }
    return false;
}

bool isValidWallPlacement(const std::vector<std::vector<Player>> &board, const Wall& wall, const std::vector<Wall>& placedWalls, int player1Row, int player2Row, int player1walls, int player2walls){
    int player1Col = Board_Size / 2;
    int player2Col = Board_Size / 2;
        if (wall.x < 0 || wall.x >= Board_Size - 1 || wall.y < 0 || wall.y >= Board_Size - 1) {
            return false;
        }
        if (isWallOverlap(placedWalls, wall)) {
            return false;
        }
        std::vector<Wall> tempWalls = placedWalls;
        tempWalls.push_back(wall);  
        if (!bfs(board, player1Row, Board_Size / 2, 0, Board_Size / 2, tempWalls) || !bfs(board, player2Row, Board_Size / 2, Board_Size - 1, Board_Size / 2, tempWalls))
        {
            return false;
        }
        return true;
    }

    bool canReachGoal(const std::vector<std::vector<Player>>& board, int startRow, int startCol, int goalRow, const std::vector<Wall>& placedWalls) {
        return bfs(board, startRow, startCol, goalRow, Board_Size / 2, placedWalls);
    }

bool canJump(const std::vector<std::vector<Player>>& board, int currentRow, int currentCol, int newRow, int newCol, int player1Row, int player2Row, int player1Col, int player2Col) {
    if (abs(currentRow - newRow) == 2 && currentCol == newCol) {
        int middleRow = (currentRow + newRow) / 2;
        if ((currentRow == player1Row && currentCol == player1Col && middleRow == player2Row) ||
            (currentRow == player2Row && currentCol == player2Col && middleRow == player1Row)) {
            return isValidMove(board, middleRow, currentCol, newRow, newCol, {}, player1Row, player2Row, player1Col, player2Col, board[currentRow][currentCol]);
        }
    } else if (abs(currentCol - newCol) == 2 && currentRow == newRow) {
        int middleCol = (currentCol + newCol) / 2;
        if ((currentRow == player1Row && currentCol == player1Col && middleCol == player2Col) ||
            (currentRow == player2Row && currentCol == player2Col && middleCol == player1Col)) {
            return isValidMove(board, currentRow, middleCol, newRow, newCol, {}, player1Row, player2Row, player1Col, player2Col, board[currentRow][currentCol]);
        }
    }
    return false;
}

bool bfs(const std::vector<std::vector<Player>>& board, int startRow, int startCol, int goalRow, int goalCol, const std::vector<Wall>& placedWalls) {
    std::vector<std::vector<bool>> visited(Board_Size, std::vector<bool>(Board_Size, false));
    std::queue<std::pair<int, int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;

    while (!q.empty()) {
        int row = q.front().first;
        int col = q.front().second;
        q.pop();
        if (row == goalRow && col == goalCol) {
            return true;
        }

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i){
            int newRow = row + dr[i];
            int newCol = col + dc[i];
            if (newRow >= 0 && newRow < Board_Size && newCol >= 0 && newCol < Board_Size && !visited[newRow][newCol] && isValidMove(board, row, col, newRow, newCol, placedWalls, startRow, goalRow, startCol, goalCol, board[startRow][startCol])) {
                visited[newRow][newCol] = true;
                q.push({newRow, newCol});
            }
        }
    }
    return false;
}

bool getPlayerMove(Player currentPlayer, int& moveRow, int& moveCol, MoveType& moveType, Direction& wallDirection) {
    char choice;
    std::cout << "Enter Move (m) or Wall (w): "; 
    std::cin >> choice;
    clearInputBuffer();
    if (choice == 'm') {
        moveType = MoveType::Move;
        std::cout << "Enter Move Row: ";
        std::cin >> moveRow;
        clearInputBuffer();
        std::cout << "Enter Move Col: ";
        std::cin >> moveCol;
        clearInputBuffer();
        return true;
    } else if (choice == 'w') {
        moveType = MoveType::Wall;
        std::cout << "Enter Wall Row: ";
        std::cin >> moveRow;
        std::cout << "Enter Wall Col: ";
        std::cin >> moveCol;
        while (true) {  
            char direction;
            std::cout << "Enter Wall Direction (h/v): ";
            std::cin >> direction;
            clearInputBuffer();
            if (direction == 'h') {
                wallDirection = Direction::horizontal;
                return true;
            } else if (direction == 'v') {
                wallDirection = Direction::vertical;
                return true;
            } 
            else {
                std::cout << "Invalid Direction! Please enter 'h' or 'v'." << std::endl;
            }
        }
        return true;
    }
    return false; // Add this line to handle the case where neither 'm' nor 'w' is entered
}
    
void play(std::vector<std::vector<Player>>& board, int& player1Walls, int& player2Walls, int& player1Row, int& player2Row, std::vector<Wall>& placedWalls) {
    int player1Col = Board_Size / 2;
    int player2Col = Board_Size / 2;
    
    Player currentPlayer = Player::player1;
    GameState gameState = GameState::running;

    while (gameState == GameState::running) {
        displayBoard(board, player1Walls, player2Walls, placedWalls);
        std::cout << (currentPlayer == Player::player1 ? "Player 1's Turn" : "Player 2's Turn") << std::endl;

        int moveRow, moveCol;
        MoveType moveType;
        Direction wallDirection;

        if (!getPlayerMove(currentPlayer, moveRow, moveCol, moveType, wallDirection)) {
            continue;
        }

        int currentRow = (currentPlayer == Player::player1) ? player1Row : player2Row;
        int currentCol = (currentPlayer == Player::player1) ? player1Col : player2Col;

        if (moveType == MoveType::Move) {
            if (isValidMove(board, currentRow, currentCol, moveRow, moveCol, placedWalls, player1Row, player2Row, player1Col, player2Col, currentPlayer)) {
            board[currentRow][currentCol] = Player::none;
            board[moveRow][moveCol] = currentPlayer;

            if (currentPlayer == Player::player1) {
                player1Row = moveRow;
                player1Col = moveCol;
            } else {
                player2Row = moveRow;
                player2Col = moveCol;
                }
        } else {
            std::cout << "Invalid Move!" << std::endl;
            continue;
        }
    } else {
        Wall newWall = {moveRow, moveCol, wallDirection};
        if ((currentPlayer == Player::player1 && player1Walls > 0) || (currentPlayer == Player::player2 && player2Walls > 0)) {
            if (isValidWallPlacement(board, newWall, placedWalls, player1Row, player2Row, player1Walls, player2Walls)) {
                placedWalls.push_back(newWall);
                if (currentPlayer == Player::player1) {
                    player1Walls--;
                } else {
                    player2Walls--;
                }
            } else {
                std::cout << "Invalid Wall Placement!" << std::endl;
                continue;
            }
        } else {
            std::cout << "No walls remaining!" << std::endl;
            continue;
        }
            std::cout << "Invalid Input!" << std::endl;
            continue;
        }
        gameState = gameStatus(board, player1Row, player2Row);
        currentPlayer = (currentPlayer == Player::player1) ? Player::player2 : Player::player1;
    }
        displayBoard(board, player1Walls, player2Walls, placedWalls);
        std::cout << (gameState == GameState::player1_win ? "Player 1 Wins!" : "Player 2 Wins!") << std::endl;
}
