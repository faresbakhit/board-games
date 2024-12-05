#ifndef _PYRAMID_X_O_H
#define _PYRAMID_X_O_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()

using namespace std;

//Pyramid Board Class
template <typename T>
class Pyramid_X_O_Board : public Board<T> {
public:
    Pyramid_X_O_Board();
    bool update_board(int x, int y, T symbol) override;
    void display_board() override;
    bool is_win() override;
    bool is_draw() override;
    bool game_is_over() override;
};

//Pyramid Player Class
template <typename T>
class X_O_Pyramid_Player : public Player<T> {
public:
    X_O_Pyramid_Player(string name, T symbol);
    void getmove(int& x, int& y) override;
};

//Random Player Class
template <typename T>
class X_O_Random_Player : public RandomPlayer<T> {
public:
    X_O_Random_Player(T symbol);
    void getmove(int& x, int& y) override;
};

// ____Implementation of Pyramid_X_O____

template <typename T>
Pyramid_X_O_Board<T>::Pyramid_X_O_Board() {
    this->rows = 3;
    this->columns = 5;
    this->board = new char*[this->rows];

    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new char[this->columns];
        for (int j = 0; j < this->columns; j++) {
            // Determine the range of valid cells in this row
            int start = (this->columns - (2 * i + 1)) / 2;
            int end = start + (2 * i + 1);

            if (j >= start && j < end) {
                this->board[i][j] = '.'; // valid empty cell
            } else {
                this->board[i][j] = '\0'; // unused/invalid positions
            }
        }
    }
    this->n_moves = 0;
}

template <typename T>
bool Pyramid_X_O_Board<T>::update_board(int x, int y, T mark) {
    //Check if (x,y) is within the board dimensions
    if (x < 0 || x >= this->rows || y < 0 || y >= this->columns) {
        return false;
    }

    // Calculate the valid range for row x
    int start = (this->columns - (2 * x + 1)) / 2;
    int end = start + (2 * x + 1);

    // Check if the move is in the valid range and the cell is empty
    if (y >= start && y < end && this->board[x][y] == '.') {
        this->board[x][y] = toupper(mark); // Place the mark ('X' or 'O')
        this->n_moves++;
        return true;
    }

    return false; 
}

template <typename T>
void Pyramid_X_O_Board<T>::display_board() {
    for (int i = 0; i < this->rows; i++) {
        int padding = (this->columns - (2 * i + 1)) / 2;
        cout << string(padding * 2, ' '); 

        for (int j = 0; j < this->columns; j++) {
            if (this->board[i][j] != '\0') {
                cout << (this->board[i][j] == '.' ? '.' : this->board[i][j]) << " ";
            }
        }
        cout << endl;
    }
}


template <typename T>
bool Pyramid_X_O_Board<T>::is_win() {
    //Check Columns
    if ((this->board[0][2] == this->board[1][2] && this->board[1][2] == this->board[2][2] && this->board[0][2] != '.')) {
        return true;
    }
    //Check Rows
    for (int i = 0; i < this->rows; i++) {
        int start = (this->columns - (2 * i + 1)) / 2;
        int end = start + (2 * i + 1);
        for (int j = start; j <= end - 3; j++) {
            if (this->board[i][j] != '.' && this->board[i][j] == this->board[i][j + 1] && this->board[i][j + 1] == this->board[i][j + 2]) {
                return true;
            }
        }
    }
    //Check Diagonals
    if (this->board[0][2] != '.' && this->board[0][2] == this->board[1][1] && this->board[1][1] == this->board[2][0]) {
        return true;
    }
    if (this->board[0][2] != '.' && this->board[0][2] == this->board[1][3] && this->board[1][3] == this->board[2][4]) {
        return true;
    }
    return false;
}

template <typename T>
bool Pyramid_X_O_Board<T>::is_draw() {
    return (this->n_moves == 9 && !is_win());
}

template <typename T>
bool Pyramid_X_O_Board<T>::game_is_over() {
    return is_win() || is_draw();
}

template <typename T>
X_O_Pyramid_Player<T>::X_O_Pyramid_Player(string name, T symbol) : Player<T>(name, symbol) {}

template <typename T>
void X_O_Pyramid_Player<T>::getmove(int& x, int& y) {
    bool valid = false;
    do {
        cout << "Enter your move (row and column, e.g., 1 3): ";
        cin >> x >> y;

        // Validate input
        if (x < 0 || x >= 3 || y < 0 || y >= 5) {
            cout << "Invalid move. Out of range. Try again.\n";
        } else {
            int start = (5 - (2 * x + 1)) / 2;
            int end = start + (2 * x + 1);
            if (y < start || y >= end) {
                cout << "Invalid move. Not within the pyramid structure. Try again.\n";
            } else {
                valid = true;
            }
        }
    } while (!valid);
}


// Constructor for X_O_Random_Player
template <typename T>
X_O_Random_Player<T>::X_O_Random_Player(T symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
}

template <typename T>
void X_O_Random_Player<T>::getmove(int& x, int& y) {
    x = rand() % this->dimension;  // Random number between 0 and 2
    y = rand() % this->dimension;
}


#endif //_PYRAMID_X_O_H