#ifndef TICTACTOE5X5_H
#define TICTACTOE5X5_H

#include "BoardGame_Classes.h"

template <typename T>
class TicTacToe5x5_Board : public Board<T> {
public:
    TicTacToe5x5_Board();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
};

template <typename T>
class TicTacToe5x5_Player : public Player<T> {
public:
    TicTacToe5x5_Player(string name, T symbol);
    void getmove(int& x, int& y);
};

//--------------------------------------- IMPLEMENTATION

#include <cctype> // for toupper()
#include <iomanip>
#include <iostream>

using namespace std;

// Constructor for TicTacToe5x5_Board
template <typename T>
TicTacToe5x5_Board<T>::TicTacToe5x5_Board()
{
    this->rows = this->columns = 5;
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
bool TicTacToe5x5_Board<T>::update_board(int x, int y, T mark)
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

    if (this->board[x][y] != mark)
        this->n_moves++;

    this->board[x][y] = toupper(mark);

    return true;
}

// Display the board and the pieces on it
template <typename T>
void TicTacToe5x5_Board<T>::display_board()
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
bool TicTacToe5x5_Board<T>::is_win()
{
    T xPlayer = 0;
    T oPlayer = 0;
    int xPlayerScore = 0, oPlayerScore = 0;

    if (this->n_moves < 24) {
        return false;
    }

    // Check horizontally, vertically, and diagonally
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (j <= 2 && (this->board[i][j] != 0)
                && (this->board[i][j] == this->board[i][j + 1])
                && (this->board[i][j + 1] == this->board[i][j + 2])) {
                if (xPlayer == 0)
                    xPlayer = this->board[i][j];
                if (this->board[i][j] == xPlayer) {
                    xPlayerScore++;
                    std::cout << "Player " << xPlayer << " +1 (H): ";
                } else {
                    if (oPlayer == 0)
                        oPlayer = this->board[i][j];
                    oPlayerScore++;
                    std::cout << "Player " << oPlayer << " +1 (H): ";
                }
                std::cout << "(" << i << ", " << j << ") "
                          << "(" << i << ", " << j + 1 << ") "
                          << "(" << i << ", " << j + 2 << ")\n";
            }
            // Vertically
            if (i <= 2 && (this->board[i][j] != 0)
                && (this->board[i][j] == this->board[i + 1][j])
                && (this->board[i + 1][j] == this->board[i + 2][j])) {
                if (xPlayer == 0)
                    xPlayer = this->board[i][j];
                if (this->board[i][j] == xPlayer) {
                    xPlayerScore++;
                    std::cout << "Player " << xPlayer << " +1 (V): ";
                } else {
                    if (oPlayer == 0)
                        oPlayer = this->board[i][j];
                    oPlayerScore++;
                    std::cout << "Player " << oPlayer << " +1 (V): ";
                }
                std::cout << "(" << i << ", " << j << ") "
                          << "(" << i + 1 << ", " << j << ") "
                          << "(" << i + 2 << ", " << j << ")\n";
            }
            // Diagonally (Left)
            if (i <= 2 && j >= 2 && (this->board[i][j] != 0)
                && (this->board[i][j] == this->board[i + 1][j - 1])
                && (this->board[i + 1][j - 1] == this->board[i + 2][j - 2])) {
                if (xPlayer == 0)
                    xPlayer = this->board[i][j];
                if (this->board[i][j] == xPlayer) {
                    xPlayerScore++;
                    std::cout << "Player " << xPlayer << " +1 (DL): ";
                } else {
                    if (oPlayer == 0)
                        oPlayer = this->board[i][j];
                    oPlayerScore++;
                    std::cout << "Player " << oPlayer << " +1 (DL): ";
                }
                std::cout << "(" << i << ", " << j << ") "
                          << "(" << i + 1 << ", " << j - 1 << ") "
                          << "(" << i + 2 << ", " << j - 2 << ")\n";
            }
            // Diagonally (Right)
            if (i <= 2 && j <= 2 && (this->board[i][j] != 0)
                && (this->board[i][j] == this->board[i + 1][j + 1])
                && (this->board[i + 1][j + 1] == this->board[i + 2][j + 2])) {
                if (xPlayer == 0)
                    xPlayer = this->board[i][j];
                if (this->board[i][j] == xPlayer) {
                    xPlayerScore++;
                    std::cout << "Player " << xPlayer << " +1 (DR): ";
                } else {
                    if (oPlayer == 0)
                        oPlayer = this->board[i][j];
                    oPlayerScore++;
                    std::cout << "Player " << oPlayer << " +1 (DR): ";
                }
                std::cout << "(" << i << ", " << j << ") "
                          << "(" << i + 1 << ", " << j + 1 << ") "
                          << "(" << i + 2 << ", " << j + 2 << ")\n";
            }
        }
    }

    std::cout << "Player " << xPlayer << " Score: " << xPlayerScore << "\n"
              << "Player " << oPlayer << " Score: " << oPlayerScore << "\n";

    return xPlayerScore != oPlayerScore;
}

// Return true if 24 moves are done and no winner
template <typename T>
bool TicTacToe5x5_Board<T>::is_draw()
{
    return (this->n_moves == 24 && !is_win());
}

template <typename T>
bool TicTacToe5x5_Board<T>::game_is_over()
{
    return is_win() || is_draw();
}

//--------------------------------------

// Constructor for TicTacToe5x5_Player
template <typename T>
TicTacToe5x5_Player<T>::TicTacToe5x5_Player(string name, T symbol)
    : Player<T>(name, symbol)
{
}

template <typename T>
void TicTacToe5x5_Player<T>::getmove(int& x, int& y)
{
    cout << "\nEnter your move x and y (0 to 4) separated by spaces: ";
    cin >> x >> y;
}

#endif // TICTACTOE5X5_H
