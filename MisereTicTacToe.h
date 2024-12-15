#ifndef MISERETICTACTOE_H
#define MISERETICTACTOE_H

#include "BoardGame_Classes.h"

template <typename T>
class MisereTicTacToe_Board : public Board<T> {
public:
    MisereTicTacToe_Board();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();

private:
    bool previousPlayWon = false;
};

template <typename T>
class MisereTicTacToe_Player : public Player<T> {
public:
    MisereTicTacToe_Player(string name, T symbol);
    void getmove(int& x, int& y);
};

template <typename T>
class MisereTicTacToe_AIPlayer : public Player<T> {
public:
    MisereTicTacToe_AIPlayer(T symbol);
    void getmove(int& x, int& y);
private:
    int calculateMinMax(T symbol, bool isMaximizing, int depth);
    std::pair<int, int> getBestMove();
};

//--------------------------------------- IMPLEMENTATION

#include <cctype> // for toupper()
#include <limits>
#include <iostream>

using namespace std;

// Constructor for MisereTicTacToe_Board
template <typename T>
MisereTicTacToe_Board<T>::MisereTicTacToe_Board()
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
bool MisereTicTacToe_Board<T>::update_board(int x, int y, T mark)
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
void MisereTicTacToe_Board<T>::display_board()
{
    cout << "\n-------------------------------";
    for (int i = 0; i < this->rows; i++) {
        cout << "\n| ";
        for (int j = 0; j < this->columns; j++) {
            cout << "(" << i << "," << j << ") ";
            cout << (this->board[i][j] ? this->board[i][j] : ' ');
            cout << " | ";
        }
        cout << "\n-------------------------------";
    }
    cout << endl;
}

// Returns true if there is any winner
template <typename T>
bool MisereTicTacToe_Board<T>::is_win()
{
    if (previousPlayWon)
        return true;

    // Check rows and columns
    for (int i = 0; i < this->rows; i++) {
        if ((this->board[i][0] == this->board[i][1]
                && this->board[i][1] == this->board[i][2]
                && this->board[i][0] != 0)
            || (this->board[0][i] == this->board[1][i]
                && this->board[1][i] == this->board[2][i]
                && this->board[0][i] != 0)) {
            previousPlayWon = true;
            return false;
        }
    }

    // Check diagonals
    if ((this->board[0][0] == this->board[1][1]
            && this->board[1][1] == this->board[2][2] && this->board[0][0] != 0)
        || (this->board[0][2] == this->board[1][1]
            && this->board[1][1] == this->board[2][0]
            && this->board[0][2] != 0)) {
        previousPlayWon = true;
        return false;
    }

    return false;
}

// Return true if 9 moves are done and no winner
template <typename T>
bool MisereTicTacToe_Board<T>::is_draw()
{
    return (this->n_moves == 9 && !is_win());
}

template <typename T>
bool MisereTicTacToe_Board<T>::game_is_over()
{
    return is_win() || is_draw();
}

//--------------------------------------

// Constructor for MisereTicTacToe_Player
template <typename T>
MisereTicTacToe_Player<T>::MisereTicTacToe_Player(string name, T symbol)
    : Player<T>(name, symbol)
{
}

template <typename T>
void MisereTicTacToe_Player<T>::getmove(int& x, int& y)
{
    cout << "\nPlease enter your move x and y (0 to 2) separated by spaces: ";
    cin >> x >> y;
}

// Constructor for TicTacToe5x5_AiPlayer
template <typename T>
MisereTicTacToe_AIPlayer<T>::MisereTicTacToe_AIPlayer(T symbol)
    : Player<T>(symbol)
{
    this->name = "AI Player";
}

template <typename T>
void MisereTicTacToe_AIPlayer<T>::getmove(int& x, int& y)
{
    std::pair<int, int> bestMove = getBestMove();
    x = bestMove.first;
    y = bestMove.second;
}

template <typename T>
int MisereTicTacToe_AIPlayer<T>::calculateMinMax(T symbol, bool isMaximizing, int depth)
{
    T opponentSymbol = (symbol == 'X') ? 'O' : 'X';
    if (this->boardPtr->is_win()) {
        return isMaximizing ? -10 + depth : 10 - depth; // Penalize winning for maximizing
    } else if (this->boardPtr->is_draw()) {
        return 0; // Neutral value for draw
    }

    int best = isMaximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (this->boardPtr->update_board(i, j, symbol)) {
                int value = calculateMinMax(opponentSymbol, !isMaximizing, depth + 1);
                this->boardPtr->update_board(i, j, 0); // Undo move

                if (isMaximizing) {
                    best = std::max(best, value);
                } else {
                    best = std::min(best, value);
                }
            }
        }
    }
    return best;
}

// Find the best move using the minimax algorithm
template <typename T>
std::pair<int, int> MisereTicTacToe_AIPlayer<T>::getBestMove()
{
    T opponentSymbol = (this->symbol == 'X') ? 'O' : 'X';

    // Check for moves that block the opponent from winning
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

    // Avoid moves that complete a line
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (this->boardPtr->update_board(i, j, this->symbol)) {
                if (!this->boardPtr->is_win()) {
                    this->boardPtr->update_board(i, j, 0); // Undo move
                    return { i, j }; // Safe move
                }
                this->boardPtr->update_board(i, j, 0); // Undo move
            }
        }
    }

    // If no special conditions, pick the first empty cell
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (this->boardPtr->update_board(i, j, this->symbol)) {
                this->boardPtr->update_board(i, j, 0); // Undo move
                return { i, j }; // Default move
            }
        }
    }

    // No valid moves found (shouldn't happen in a properly managed game)
    return { -1, -1 };
}

#endif // MISERETICTACTOE_H
