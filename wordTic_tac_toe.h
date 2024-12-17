#ifndef _wordTic_tac_toe_H
#define _wordTic_tac_toe_H

#include "BoardGame_Classes.h"
#include <set>

template <typename T>
class word_Board:public Board<T> {
public:
    word_Board ();
    bool update_board (int x , int y , T symbol);
    void display_board () ;
    bool is_win() ;
    bool is_draw();
    bool game_is_over();

    // Set to store valid words
    set<string> validWords;
    // Load valid words from a file
    void loadValidWords();  
    // Check if the word formed is valid
    bool isValidWord(const string& word);  

};

template <typename T>
class word_Player : public Player<T> {
private:
    T currentSymbol; //to store the symbol
public:
    word_Player (string name, T symbol);
    void getmove(int& x, int& y) ;
    T getsymbol();

};

template <typename T>
class word_Random_Player : public RandomPlayer<T>{
private:
    T currentSymbol; //to store the symbol
public:
    word_Random_Player (T symbol);
    void getmove(int &x, int &y) ;
    T getsymbol() ;
};


//---------- IMPLEMENTATION--------------

#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()
#include <fstream>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

// Constructor for word_Board
template <typename T>
word_Board<T>::word_Board() {
    loadValidWords();
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
bool word_Board<T>::update_board(int x, int y, T mark) {
    // Only update if move is valid
    if (!(x < 0 || x >= this->rows || y < 0 || y >= this->columns) && (this->board[x][y] == 0|| mark == 0)) {
        cout << "Updating board at (" << x << "," << y << ") with symbol: " << mark << endl; 
        if (mark == 0){
            this->n_moves--;
            this->board[x][y] = 0;
        }
        else {
            this->n_moves++;
            this->board[x][y] = mark;
        }
        //check if there is a valid word formed after each move
        if (is_win()) {
            cout << "Valid word formed!\n";
            return true;
        }
        return true;
    }
    return false;
}

// Display the board and the pieces on it
template <typename T>
void word_Board<T>::display_board() {
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

// Returns true if there is any winner
template <typename T>
bool word_Board<T>::is_win() {
    // Check rows
    for (int i = 0; i < this->rows; i++) {
        string rowWord = "", reverseRowWord = "";
        for (int j = 0; j < this->columns; j++) {
            rowWord += this->board[i][j];
            reverseRowWord = this->board[i][j] + reverseRowWord;
        }
        if (isValidWord(rowWord) || isValidWord(reverseRowWord)) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < this->columns; j++) {
        string colWord = "", reverseColWord = "";
        for (int i = 0; i < this->rows; i++) {
            colWord += this->board[i][j];
            reverseColWord = this->board[i][j] + reverseColWord;
        }
        if (isValidWord(colWord) || isValidWord(reverseColWord)) {
            return true;
        }
    }

    // Check diagonal 1 
    string diag1Word = "", reverseDiag1Word = "";
    for (int i = 0; i < this->rows; i++) {
        diag1Word += this->board[i][i];
        reverseDiag1Word = this->board[i][i] + reverseDiag1Word;
    }
    if (isValidWord(diag1Word) || isValidWord(reverseDiag1Word)) {
        return true;
    }

    // Check diagonal 2 
    string diag2Word = "", reverseDiag2Word = "";
    for (int i = 0; i < this->rows; i++) {
        diag2Word += this->board[i][this->columns - i - 1];
        reverseDiag2Word = this->board[i][this->columns - i - 1] + reverseDiag2Word;
    }
    if (isValidWord(diag2Word) || isValidWord(reverseDiag2Word)) {
        return true;
    }

    return false; 
}

// Return true if 9 moves are done and no winner
template <typename T>
bool word_Board<T>::is_draw() {
    return (this->n_moves == 9 && !is_win());
}

template <typename T>
bool word_Board<T>::game_is_over() {
    return is_win() || is_draw();
}

// Load words from the file
template <typename T>
void word_Board<T>::loadValidWords() {
    string filename = "dic.txt";
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return;
    }

    string word;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::toupper);
        this->validWords.insert(word); // Insert into set 
    }

    file.close();
}

// Check if the word formed is valid
template <typename T>
bool word_Board<T>::isValidWord(const string& word) {
    return validWords.find(word) != validWords.end();  
}

//--------------------------------------

// Constructor for word_Player
template <typename T>
word_Player<T>:: word_Player(string name, T symbol) : Player<T>(name, symbol) {
    this->currentSymbol = symbol;  // Set the default symbol
}

//Override the getsymbol() method to return the dynamically chosen symbol (currentSymbol).
template <typename T>
T word_Player<T>::getsymbol() {
    return this->currentSymbol;  // Return the symbol chosen for the current move
}

template <typename T>
void word_Player<T>::getmove(int& x, int& y) {
    cout << "\nPlease enter your move x and y (0 to 2) separated by spaces: ";
    cin >> x >> y;
    //input the symbol 
    cout << "Enter the letter you want for this move: ";
    cin >> this->currentSymbol;
    // Convert the symbol to uppercase
    this->currentSymbol = toupper(this->currentSymbol);

    // Loop to ensure a valid symbol
    while (!isalpha(this->currentSymbol)) {
        cout << "Invalid symbol! Please use a letter. Try again.\n";
        cout << "Enter the letter you want for this move: ";
        cin >> this->currentSymbol;
        this->currentSymbol = toupper(this->currentSymbol);
    }
    this->symbol=currentSymbol;
}

// Constructor for X_O_Random_Player
template <typename T>
word_Random_Player<T>::word_Random_Player(T symbol) : RandomPlayer<T>(symbol) {
    this->dimension = 3;
    this->name = "Random Computer Player";
    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
}

template <typename T>
void word_Random_Player<T>::getmove(int& x, int& y) {
    x = rand() % this->dimension;  // Random number between 0 and 2
    y = rand() % this->dimension;
    //Generate a random letter 
    this->currentSymbol = 'A' + (rand() % 26); 
    this->symbol=currentSymbol;
}

#endif //_wordTic_tac_toe_H

