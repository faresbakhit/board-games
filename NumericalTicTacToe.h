#ifndef _NUMERICAL_TICTACTOE_H
#define _NUMERICAL_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

template <typename T>
class NumericalTicTacToeBoard : public Board<T> {
public:
    NumericalTicTacToeBoard();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
private:
    set<T> used_numbers;
    bool check_sum(T a, T b, T c);
};

// Player classes
template <typename T>
class NumericalTicTacToePlayer : public Player<T> {
public:
    NumericalTicTacToePlayer(string name, T symbol);
    void getmove(int& x, int& y);
};

template <typename T>
class NumericalTicTacToeRandomPlayer : public Player<T> {
public:
    NumericalTicTacToeRandomPlayer(T symbol);
    void getmove(int& x, int& y);
};

// ---------------------- IMPLEMENTATION ----------------------

// Board Implementation
template <typename T>
NumericalTicTacToeBoard<T>::NumericalTicTacToeBoard() {
    this->rows = this->columns = 3;
    this->board = new T*[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new T[this->columns]{0}; // Initialize with 0
    }
    this->n_moves = 0;
}

template <typename T>
bool NumericalTicTacToeBoard<T>::update_board(int x, int y, T symbol) {
    // Check if the position is within bounds
    if (x < 0 || x >= this->rows || y < 0 || y >= this->columns) {
        cout << "Invalid move: Position (" << x << ", " << y << ") is out of bounds.\n";
        return false; // Invalid move
    }

    // Check if the position is empty
    if (this->board[x][y] != 0) {
        cout << "Invalid move: Position (" << x << ", " << y << ") is already occupied.\n";
        return false; // Invalid move
    }

    // Check if the number is in the valid range [1, 9]
    if (symbol < 1 || symbol > 9) {
        cout << "Invalid move: The number must be in the range [1, 9].\n";
        return false; // Invalid number
    }

    // Check if the number matches the turn (odd/even)
    if ((this->n_moves % 2 == 0 && symbol % 2 == 0)) {
        cout << "Invalid move: Player " << ((this->n_moves % 2 == 0) ? "1" : "2")
             << " must use an odd number on this turn.\n";
        return false; // Number does not match the turn
    }
    if ((this->n_moves % 2 != 0 && symbol % 2 != 0)) {
        cout << "Invalid move: Player " << ((this->n_moves % 2 == 0) ? "1" : "2")
             << " must use an even number on this turn.\n";
        return false; // Number does not match the turn
    }

    // Check if the number has already been used
    if (used_numbers.count(symbol)) {
        cout << "Invalid move: The number " << symbol << " has already been used.\n";
        return false; // Number already used
    }

    // Update the board and record the move
    this->board[x][y] = symbol;
    this->n_moves++;
    used_numbers.insert(symbol);

    cout << "Move accepted: Player " << ((this->n_moves % 2 == 0) ? "2" : "1")
         << " placed " << symbol << " at position (" << x << ", " << y << ").\n";
    return true;
}

template <typename T>
void NumericalTicTacToeBoard<T>::display_board() {
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << this->board[i][j] << " |";
        }
        cout << "\n-----------------------------";
    }
    cout << endl;
}

template <typename T>
bool NumericalTicTacToeBoard<T>::is_win() {
    for (int i = 0; i < 3; i++) {
        if (check_sum(this->board[i][0], this->board[i][1], this->board[i][2]) ||
            check_sum(this->board[0][i], this->board[1][i], this->board[2][i])) {
            return true;
        }
    }
    return check_sum(this->board[0][0], this->board[1][1], this->board[2][2]) ||
           check_sum(this->board[0][2], this->board[1][1], this->board[2][0]);
}

template <typename T>
bool NumericalTicTacToeBoard<T>::is_draw() {
    return this->n_moves == 9 && !is_win();
}

template <typename T>
bool NumericalTicTacToeBoard<T>::game_is_over() {
    return is_win() || is_draw();
}

template <typename T>
bool NumericalTicTacToeBoard<T>::check_sum(T a, T b, T c) {
    return (a != 0 && b != 0 && c != 0 && a + b + c == 15);
}

// Player Implementation
template <typename T>
NumericalTicTacToePlayer<T>::NumericalTicTacToePlayer(string name, T symbol)
    : Player<T>(name, symbol) {}

template <typename T>
void NumericalTicTacToePlayer<T>::getmove(int& x, int& y) {
    cout << this->name << ", enter your move as row, and column (row col): ";
    cin >> x >> y;
    T num;
    cout << "Enter the number:";
    cin >> num;
    this->symbol=num;
}

// Constructor for Random Player
template<typename T>
NumericalTicTacToeRandomPlayer<T>::NumericalTicTacToeRandomPlayer(T symbol) : Player<T>(symbol) {
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
}

template <typename T>
void NumericalTicTacToeRandomPlayer<T>::getmove(int& x, int& y) {
    x = rand() % 3;
    y = rand() % 3;
    T num;
    if (this->symbol == 1) {
        num = (rand() % 5) * 2 + 1;
    } else {
        num = (rand() % 5) * 2 + 2;
    }
    this->symbol = num;
}

#endif //_NUMERICAL_TICTACTOE_H
