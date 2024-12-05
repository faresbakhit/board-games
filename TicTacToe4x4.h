#ifndef TICTACTOE4X4_H
#define TICTACTOE4X4_H

#include "BoardGame_Classes.h"

template <typename T>
class TicTacToe4x4_Board : public Board<T> {
public:
    TicTacToe4x4_Board();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
};

template <typename T>
class TicTacToe4x4_Player : public Player<T> {
public:
    TicTacToe4x4_Player(string name, T symbol);
    void getmove(int& x, int& y);
};

//--------------------------------------- IMPLEMENTATION

#include <cctype> // for toupper()
#include <iomanip>
#include <iostream>

using namespace std;

// Constructor for TicTacToe4x4_Board
template <typename T>
TicTacToe4x4_Board<T>::TicTacToe4x4_Board()
{
    this->rows = this->columns = 4;
    this->board = new char*[this->rows];
    this->board[0] = new char[this->columns];
    this->board[this->rows - 1] = new char[this->columns];
    for (int j = 0; j < this->columns; j++) {
        this->board[0][j] = 'O' + (j % 2) * ('X' - 'O');
        this->board[this->rows - 1][j] = 'X' - (j % 2) * ('X' - 'O');
    }
    for (int i = 1; i < this->rows - 1; i++) {
        this->board[i] = new char[this->columns];
        for (int j = 0; j < this->columns; j++) {
            this->board[i][j] = 0;
        }
    }
    this->n_moves = 0;
}

template <typename T>
bool TicTacToe4x4_Board<T>::update_board(int x, int y, T mark)
{
    // Only update if move is valid
    if (x < 0 || x >= this->rows || y < 0 || y >= this->columns) {
        // Out of bounds
        return false;
    }

    if (this->board[x][y] != 0) {
        // Tokens may not jump over other tokens
        return false;
    }

    mark = toupper(mark);
    const std::pair<int, int> directions[]
        = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
    for (const auto& d : directions) {
        int i = x + d.first;
        int j = y + d.second;
        if (i < 0 || i >= this->rows || j < 0 || j >= this->columns) {
            // Out of bounds
            continue;
        }
        if (this->board[i][j] == mark) {
            this->board[i][j] = 0;
            this->board[x][y] = mark;
            return true;
        }
    }

    return false;
}

// Display the board and the pieces on it
template <typename T>
void TicTacToe4x4_Board<T>::display_board()
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
bool TicTacToe4x4_Board<T>::is_win()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // Horizontally
            if (j <= 1 && (this->board[i][j] != 0)
                && (this->board[i][j] == this->board[i][j + 1])
                && (this->board[i][j + 1] == this->board[i][j + 2])) {
                return true;
            }
            // Vertically
            if (i <= 1 && (this->board[i][j] != 0)
                && (this->board[i][j] == this->board[i + 1][j])
                && (this->board[i + 1][j] == this->board[i + 2][j])) {
                return true;
            }
            // Diagonally (Left)
            if (i <= 1 && j >= 1 && (this->board[i][j] != 0)
                && (this->board[i][j] == this->board[i + 1][j - 1])
                && (this->board[i + 1][j - 1] == this->board[i + 2][j - 2])) {
                return true;
            }
            // Diagonally (Right)
            if (i <= 1 && j <= 1 && (this->board[i][j] != 0)
                && (this->board[i][j] == this->board[i + 1][j + 1])
                && (this->board[i + 1][j + 1] == this->board[i + 2][j + 2])) {
                return true;
            }
        }
    }
    return false;
}

// Return true if 9 moves are done and no winner
template <typename T>
bool TicTacToe4x4_Board<T>::is_draw() { return false; }

template <typename T>
bool TicTacToe4x4_Board<T>::game_is_over()
{
    return is_win();
}

//--------------------------------------

// Constructor for TicTacToe5x5_Player
template <typename T>
TicTacToe4x4_Player<T>::TicTacToe4x4_Player(string name, T symbol)
    : Player<T>(name, symbol)
{
}

template <typename T>
void TicTacToe4x4_Player<T>::getmove(int& x, int& y)
{
    cout << "\nEnter your move x and y (0 to 3) separated by spaces: ";
    cin >> x >> y;
}

#endif // TICTACTOE4X4_H
