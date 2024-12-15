#ifndef ULTIMATETICTACTOE_H
#define ULTIMATETICTACTOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>

using namespace std;

// Ultimate Tic Tac Toe Board Class
template <typename T>
class UltimateTicTacToeBoard : public Board<T> {
public:
    UltimateTicTacToeBoard();
    bool update_board(int x, int y, T symbol);
    void display_board();
    bool is_win();
    bool is_draw();
    bool game_is_over();
private:
    vector<vector<T>> sub_winners;
    bool check_sub_board_win(int subgrid_row, int subgrid_col, T symbol);
    bool check_main_board_win(T symbol);
};

// Player Class for Ultimate Tic Tac Toe
template <typename T>
class UltimateTicTacToePlayer : public Player<T> {
public:
    UltimateTicTacToePlayer(string name, T symbol);
    void getmove(int& x, int& y);
};

// Random Player Class for Ultimate Tic Tac Toe
template <typename T>
class UltimateTicTacToeRandomPlayer : public RandomPlayer<T> {
public:
    UltimateTicTacToeRandomPlayer(T symbol);
    void getmove(int& x, int& y);
};

// ----------Implementation-----------

// Board Implementation
template<typename T>
UltimateTicTacToeBoard<T>::UltimateTicTacToeBoard() {
    this->rows = this->columns = 9;
    this->board = new T*[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->board[i] = new T[this->columns];
        for (int j = 0; j < this->columns; j++) {
            this->board[i][j] = 0;
        }
    }
    this->n_moves = 0;
    //2D vector to store the winners of the sub-boards
    sub_winners = vector<vector<T>>(3, vector<T>(3, 0));
}

template<typename T>
bool UltimateTicTacToeBoard<T>::check_sub_board_win(int subgrid_row, int subgrid_col, T symbol) {
    //to get the start of row and column in the subgrids
    int start_row = subgrid_row * 3;
    int start_col = subgrid_col * 3;
    
    for (int i = 0; i < 3; i++) {
        //check rows
        if (this->board[start_row + i][start_col] == symbol &&
            this->board[start_row + i][start_col + 1] == symbol &&
            this->board[start_row + i][start_col + 2] == symbol)
            return true;
        //check columns
        if (this->board[start_row][start_col + i] == symbol &&
            this->board[start_row + 1][start_col + i] == symbol &&
            this->board[start_row + 2][start_col + i] == symbol)
            return true;
    }
    //Check diagonals
    if (this->board[start_row][start_col] == symbol &&
        this->board[start_row + 1][start_col + 1] == symbol &&
        this->board[start_row + 2][start_col + 2] == symbol){
            return true;
        }
        
    if (this->board[start_row][start_col + 2] == symbol &&
        this->board[start_row + 1][start_col + 1] == symbol &&
        this->board[start_row + 2][start_col] == symbol)
        return true;
    return false;
}

template<typename T>
bool UltimateTicTacToeBoard<T>::check_main_board_win(T symbol) {
    //Check rows 
    for (int i = 0; i < 3; i++) {
        if (sub_winners[i][0] == symbol && sub_winners[i][1] == symbol && sub_winners[i][2] == symbol)
            return true;
    }
    //Check columns
    for (int j = 0; j < 3; j++) {
        if (sub_winners[0][j] == symbol && sub_winners[1][j] == symbol && sub_winners[2][j] == symbol)
            return true;
    }
    //Check diagonals
    if (sub_winners[0][0] == symbol && sub_winners[1][1] == symbol && sub_winners[2][2] == symbol)
        return true;
    if (sub_winners[0][2] == symbol && sub_winners[1][1] == symbol && sub_winners[2][0] == symbol)
        return true;
    
    return false;
}

template<typename T>
bool UltimateTicTacToeBoard<T>::update_board(int x, int y, T symbol) {
    if (x >= 0 && x < this->rows && y >= 0 && y < this->columns && this->board[x][y] == 0) {
        this->board[x][y] = symbol;
        this->n_moves++;
        int subgrid_row = x / 3;
        int subgrid_col = y / 3;
        if (sub_winners[subgrid_row][subgrid_col] == 0 && check_sub_board_win(subgrid_row, subgrid_col, symbol)) {
            sub_winners[subgrid_row][subgrid_col] = symbol;
        }
        return true;
    }
    return false;
}

template<typename T>
void UltimateTicTacToeBoard<T>::display_board() {
    // Print column headers
    cout << "    ";
    for (int j = 0; j < this->columns; j++) {
        if (j % 3 == 0 && j != 0) cout << "  "; // Extra space for 3x3 grouping
        cout << j << " ";
    }
    cout << "\n";

    for (int i = 0; i < this->rows; i++) {
        if (i % 3 == 0) cout << "   +-------+-------+-------+\n";
        
        // Print row index on the left
        cout << i << "  ";
        
        for (int j = 0; j < this->columns; j++) {
            if (j % 3 == 0) cout << "| ";
            cout << (this->board[i][j] == 0 ? ' ' : this->board[i][j]) << " ";
        }
        cout << "|\n";
    }
    cout << "   +-------+-------+-------+\n";
}

template<typename T>
bool UltimateTicTacToeBoard<T>::is_win() {
    return check_main_board_win('X') || check_main_board_win('O');
}

template<typename T>
bool UltimateTicTacToeBoard<T>::is_draw() {
    return this->n_moves == 81 && !is_win();
}

template<typename T>
bool UltimateTicTacToeBoard<T>::game_is_over() {
    return is_win() || is_draw();
}

// Player Implementation
template <typename T>
UltimateTicTacToePlayer<T>::UltimateTicTacToePlayer(string name, T symbol)
    : Player<T>(name, symbol) {}

template <typename T>
void UltimateTicTacToePlayer<T>::getmove(int& x, int& y) {
    cout << this->name << ", enter your move (row and column): ";
    cin >> x >> y;
}

// Random Player Implementation
template <typename T>
UltimateTicTacToeRandomPlayer<T>::UltimateTicTacToeRandomPlayer(T symbol)
    : RandomPlayer<T>(symbol) {
    srand(static_cast<unsigned int>(time(0)));
}

template <typename T>
void UltimateTicTacToeRandomPlayer<T>::getmove(int& x, int& y) {
    x = rand() % 9;
    y = rand() % 9;
}

#endif