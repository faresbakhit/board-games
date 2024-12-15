#ifndef _FOUR_IN_A_ROW_H
#define _FOUR_IN_A_ROW_H

#include "BoardGame_Classes.h"
#include <bits/stdc++.h>

using namespace std;

template <typename T>
class FourInARowBoard : public Board<T> {
public:
    FourInARowBoard();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
};

template <typename T>
class FourInARowPlayer : public Player<T> {
public:
    FourInARowPlayer(string name, T symbol);
    void getmove(int& x, int& y);
};

template <typename T>
class FourInARowRandomPlayer : public RandomPlayer<T> {
public:
    FourInARowRandomPlayer(T symbol);
    void getmove(int& x, int& y);
};

//--------------------------------------- IMPLEMENTATION

// Constructor for FourInARowBoard
template <typename T>
FourInARowBoard<T>::FourInARowBoard() {
    this->rows = 6;
    this->columns = 7;
    this->board = new T*[this->rows];
    for (int i = 0; i < this->rows; ++i) {
        this->board[i] = new T[this->columns];
        for (int j = 0; j < this->columns; ++j) {
            this->board[i][j] = ' ';
        }
    }
    this->n_moves = 0;
}

template <typename T>
bool FourInARowBoard<T>::update_board(int x, int y, T symbol) {
    // Validate column range
    if (y < 0 || y >= this->columns) {
        cout << "Invalid move: Column " << y << " is out of bounds.\n";
        return false; // Invalid move
    }
    // Find the lowest available row in column y
    for (int i = 0; i < this->rows; ++i) {
        if (this->board[i][y] == ' ') { // Empty cell found
            this->board[i][y] = symbol;
            ++this->n_moves; // Increment move count
            return true;
        }
    }
    // If no empty row was found
    cout << "Invalid move: Column " << y << " is full.\n";
    return false; // Column is full
}

template <typename T>
void FourInARowBoard<T>::display_board() {
    for (int i = this->rows - 1; i >= 0; --i) {
        for (int j = 0; j < this->columns; ++j) {
            cout << "| " << this->board[i][j] << " ";
        }
        cout << "|\n";
    }
    cout << "-----------------------------\n";
    cout << "  0   1   2   3   4   5   6  \n";
}

template <typename T>
bool FourInARowBoard<T>::is_win() {
    // Horizontal check
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j <= this->columns - 4; ++j) {
            if (this->board[i][j] != ' ' &&
                this->board[i][j] == this->board[i][j + 1] &&
                this->board[i][j] == this->board[i][j + 2] &&
                this->board[i][j] == this->board[i][j + 3]) {
                return true;
            }
        }
    }

    // Vertical check
    for (int i = 0; i <= this->rows - 4; ++i) {
        for (int j = 0; j < this->columns; ++j) {
            if (this->board[i][j] != ' ' &&
                this->board[i][j] == this->board[i + 1][j] &&
                this->board[i][j] == this->board[i + 2][j] &&
                this->board[i][j] == this->board[i + 3][j]) {
                return true;
            }
        }
    }

    // Diagonal check (\ direction)
    for (int i = 0; i <= this->rows - 4; ++i) {
        for (int j = 0; j <= this->columns - 4; ++j) {
            if (this->board[i][j] != ' ' &&
                this->board[i][j] == this->board[i + 1][j + 1] &&
                this->board[i][j] == this->board[i + 2][j + 2] &&
                this->board[i][j] == this->board[i + 3][j + 3]) {
                return true;
            }
        }
    }

    // Diagonal check (/ direction)
    for (int i = 3; i < this->rows; ++i) {
        for (int j = 0; j <= this->columns - 4; ++j) {
            if (this->board[i][j] != ' ' &&
                this->board[i][j] == this->board[i - 1][j + 1] &&
                this->board[i][j] == this->board[i - 2][j + 2] &&
                this->board[i][j] == this->board[i - 3][j + 3]) {
                return true;
            }
        }
    }

    return false;
}

template <typename T>
bool FourInARowBoard<T>::is_draw() {
    return (this->n_moves == this->rows * this->columns && !is_win());
}

template <typename T>
bool FourInARowBoard<T>::game_is_over() {
    return is_win() || is_draw();
}

//--------------------------------------

// Constructor for FourInARowPlayer
template <typename T>
FourInARowPlayer<T>::FourInARowPlayer(string name, T symbol) : Player<T>(name, symbol) {}

// Get move from human player
template <typename T>
void FourInARowPlayer<T>::getmove(int& x, int& y) {
    cout << this->name << " (" << this->symbol << "), enter column (0-6): ";
    cin >> y;
    x = 0;
}

// Constructor for FourInARowRandomPlayer
template <typename T>
FourInARowRandomPlayer<T>::FourInARowRandomPlayer(T symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 7;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));
}

// Get random move
template <typename T>
void FourInARowRandomPlayer<T>::getmove(int& x, int& y) {
    y = rand() % this->dimension; // Pick a random column
    x = 0;
}
#endif // FOUR_IN_A_ROW_H