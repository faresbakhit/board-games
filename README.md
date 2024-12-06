# Board Games

> CS213: OOP (2024-2025) Assignment 2 – Version 3.0

Games description is [_here_](misc/CS213-2023-2024-Assignment2-V4.0.pdf)

|Student Name|Student ID|Games|
|--|--|--|
|Gaeza|20230105|1 & 4|
|Rofida|20230141|2 & 5|
|Fares|20230277|3 & 6|
|*Team*| |7 & 8|

## Graphical User Interface

To build the project's GUI, make sure to have CMake installed locally.

### Desktop

```
mkdir build
cd build
cmake ..
make
```

### Web

```
mkdir build
cd build
emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release
emmake make
```

## Board Game Classes

This C++ code defines a set of classes for implementing a generic board game. The code uses templates to allow for different board game types and player symbols.

### Board Class

The `Board` class is an abstract base class that defines the interface for a board game board. It has the following members:

-   `rows`: the number of rows in the board
-   `columns`: the number of columns in the board
-   `board`: a 2D array to store the board state
-   `n_moves`: the number of moves made on the board

The `Board` class defines the following pure virtual methods:

-   `update_board(int x, int y, T symbol)`: updates the board with the given symbol at the specified coordinates, returning `true` if the move is valid and `false` otherwise
-   `display_board()`: displays the current state of the board
-   `is_win()`: returns `true` if there is a winner on the board
-   `is_draw()`: returns `true` if the game is a draw (all moves are made and no winner)
-   `game_is_over()`: returns `true` if the game is over (either a winner or a draw)

### Player Class

The `Player` class is an abstract base class that defines the interface for a player in the board game. It has the following members:

-   `name`: the name of the player
-   `symbol`: the symbol used by the player on the board
-   `boardPtr`: a pointer to the board object

The `Player` class defines the following methods:

-   `getmove(int& x, int& y)`: a pure virtual method that allows the player to specify their next move
-   `getsymbol()`: returns the player's symbol
-   `getname()`: returns the player's name
-   `setBoard(Board<T>* b)`: sets the board pointer for the player

### RandomPlayer Class

The `RandomPlayer` class is a derived class of the `Player` class that represents a random computer player. It has the following additional member:

-   `dimension`: the size of the board

The `RandomPlayer` class overrides the `getmove(int& x, int& y)` method to generate a random move on the board.

### GameManager Class

The `GameManager` class is responsible for managing the game flow. It has the following members:

-   `boardPtr`: a pointer to the board object
-   `players`: an array of two player objects

The `GameManager` class defines the following method:

-   `run()`: the main game loop that alternates between the two players, updates the board, and checks for a winner or a draw.