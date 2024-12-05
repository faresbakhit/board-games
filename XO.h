#ifndef XO_H
#define XO_H

#include "BoardGame_Classes.h"

template <typename T>
class XO_Board : public Board<T> {
public:
    XO_Board();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
};

template <typename T>
class XO_Player : public Player<T> {
public:
    XO_Player(string name, T symbol);
    void getmove(int& x, int& y);
};

template <typename T>
class XO_Random_Player : public RandomPlayer<T> {
public:
    XO_Random_Player(T symbol);
    void getmove(int& x, int& y);
};

template <typename T>
class XO_MinMax_Player : public Player<T> {
public:
    XO_MinMax_Player(T symbol);

    void getmove(int& x, int& y) override;

private:
    int calculateMinMax(T s, bool isMaximizing);
    std::pair<int, int> getBestMove();
};

//--------------------------------------- IMPLEMENTATION

#include <algorithm> // For std::max and std::min
#include <cctype> // for toupper()
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

// Constructor for X_O_Board
template <typename T>
XO_Board<T>::XO_Board()
{
    this->rows = this->columns = 3;
    this->board = new char*[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new char[this->columns];
        for (int j = 0; j < this->columns; j++) {
            this->board[i][j] = 0;
        }
    }
    this->n_moves = 0;
}

template <typename T>
bool XO_Board<T>::update_board(int x, int y, T mark)
{
    // Only update if move is valid
    if (!(x < 0 || x >= this->rows || y < 0 || y >= this->columns)
        && (this->board[x][y] == 0 || mark == 0)) {
        if (mark == 0) {
            this->n_moves--;
            this->board[x][y] = 0;
        } else {
            this->n_moves++;
            this->board[x][y] = toupper(mark);
        }

        return true;
    }
    return false;
}

// Display the board and the pieces on it
template <typename T>
void XO_Board<T>::display_board()
{
    cout << "\n-";
    for (int i = 0; i < this->rows; i++) {
        cout << "----------";
    }
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ") ";
            cout << (this->board[i][j] ? this->board[i][j] : ' ');
            cout << " | ";
        }
        cout << "\n-";
        for (int i = 0; i < this->rows; i++) {
            cout << "----------";
        }
    }
    cout << endl;
}

// Returns true if there is any winner
template <typename T>
bool XO_Board<T>::is_win()
{
    // Check rows and columns
    for (int i = 0; i < this->rows; i++) {
        if ((this->board[i][0] == this->board[i][1]
                && this->board[i][1] == this->board[i][2]
                && this->board[i][0] != 0)
            || (this->board[0][i] == this->board[1][i]
                && this->board[1][i] == this->board[2][i]
                && this->board[0][i] != 0)) {
            return true;
        }
    }

    // Check diagonals
    if ((this->board[0][0] == this->board[1][1]
            && this->board[1][1] == this->board[2][2] && this->board[0][0] != 0)
        || (this->board[0][2] == this->board[1][1]
            && this->board[1][1] == this->board[2][0]
            && this->board[0][2] != 0)) {
        return true;
    }

    return false;
}

// Return true if 9 moves are done and no winner
template <typename T>
bool XO_Board<T>::is_draw()
{
    return (this->n_moves == 9 && !is_win());
}

template <typename T>
bool XO_Board<T>::game_is_over()
{
    return is_win() || is_draw();
}

//--------------------------------------

// Constructor for X_O_Player
template <typename T>
XO_Player<T>::XO_Player(string name, T symbol)
    : Player<T>(name, symbol)
{
}

template <typename T>
void XO_Player<T>::getmove(int& x, int& y)
{
    cout << "\nEnter your move x and y (0 to 2) separated by spaces: ";
    cin >> x >> y;
}

// Constructor for X_O_Random_Player
template <typename T>
XO_Random_Player<T>::XO_Random_Player(T symbol)
    : RandomPlayer<T>(symbol)
{
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(
        static_cast<unsigned int>(time(0))); // Seed the random number generator
}

template <typename T>
void XO_Random_Player<T>::getmove(int& x, int& y)
{
    x = rand() % this->dimension; // Random number between 0 and 2
    y = rand() % this->dimension;
}

// Constructor for the templated class
template <typename T>
XO_MinMax_Player<T>::XO_MinMax_Player(T symbol)
    : Player<T>(symbol)
{
    this->name = "AI Player";
}

// Method to get the best move for the player
template <typename T>
void XO_MinMax_Player<T>::getmove(int& x, int& y)
{
    std::pair<int, int> bestMove = getBestMove();
    x = bestMove.first;
    y = bestMove.second;
}

// Minimax algorithm to evaluate the board
template <typename T>
int XO_MinMax_Player<T>::calculateMinMax(T s, bool isMaximizing)
{
    if (this->boardPtr->is_win()) {
        return isMaximizing ? -1 : 1;
    } else if (this->boardPtr->is_draw()) {
        return 0;
    }

    int bestValue = isMaximizing ? std::numeric_limits<int>::min()
                                 : std::numeric_limits<int>::max();
    T opponentSymbol = (s == 'X') ? 'O' : 'X';

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (this->boardPtr->update_board(i, j, s)) {
                int value = calculateMinMax(opponentSymbol, !isMaximizing);
                this->boardPtr->update_board(i, j, 0); // Undo move

                if (isMaximizing) {
                    bestValue = std::max(bestValue, value);
                } else {
                    bestValue = std::min(bestValue, value);
                }
            }
        }
    }

    return bestValue;
}

// Find the best move using the minimax algorithm
template <typename T>
std::pair<int, int> XO_MinMax_Player<T>::getBestMove()
{
    int bestValue = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = { -1, -1 };
    T opponentSymbol = (this->symbol == 'X') ? 'O' : 'X';

    // First, check if we can win in the next move
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (this->boardPtr->update_board(i, j, this->symbol)) {
                if (this->boardPtr->is_win()) {
                    this->boardPtr->update_board(i, j, 0); // Undo move
                    return { i, j }; // Winning move found
                }
                this->boardPtr->update_board(i, j, 0); // Undo move
            }
        }
    }

    // Second, check if the opponent can win in their next move and block them
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (this->boardPtr->update_board(i, j, opponentSymbol)) {
                if (this->boardPtr->is_win()) {
                    this->boardPtr->update_board(i, j, 0); // Undo move
                    return { i, j }; // Block opponent's winning move
                }
                this->boardPtr->update_board(i, j, 0); // Undo move
            }
        }
    }

    // If no immediate win or block, use MinMax to find the best move
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (this->boardPtr->update_board(i, j, this->symbol)) {
                int moveValue = calculateMinMax(this->symbol, false);
                this->boardPtr->update_board(i, j, 0); // Undo move

                if (moveValue > bestValue) {
                    bestMove = { i, j };
                    bestValue = moveValue;
                }
            }
        }
    }

    return bestMove;
}

#endif // XO_H
