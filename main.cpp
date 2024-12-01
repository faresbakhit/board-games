#include <iostream>
#include "BoardGame_Classes.h"
#include "3x3X_O.h"
#include "MinMaxPlayer.h"
#include "TicTacToe5x5.h"
#include "MisereTicTacToe.h"

using namespace std;

int main() {
    int choice, gameChoice;
    Player<char>* players[2];
    Board<char>* board;
    string playerXName, player2Name;

    cout << "Welcome to board-games.\n";
    cout << "Enter board game type:\n";
    cout << "0. 3x3 XO\n";
    cout << "3. 5x5 Tic Tac Toe\n";
    cout << "6. Misere Tic Tac Toe\n";
    cin >> gameChoice;

    switch(gameChoice) {
        case 0:
            board = new X_O_Board<char>();
            break;
        case 3:
            board = new TicTacToe5x5_Board<char>();
            break;
        case 6:
            board = new MisereTicTacToe_Board<char>();
            break;
        default:
            cout << "Invalid choice for board game type. Exiting the game.\n";
            return 1;
    }

    // Set up player 1
    cout << "Enter Player X name: ";
    cin >> playerXName;
    cout << "Choose Player X type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cin >> choice;

    switch(gameChoice) {
        case 0:
            switch(choice) {
                case 1:
                    players[0] = new X_O_Player<char>(playerXName, 'X');
                    break;
                case 2:
                    players[0] = new X_O_Random_Player<char>('X');
                    break;
                case 3:
                    players[0] = new X_O_MinMax_Player<char>('X');
                    players[0]->setBoard(board);
                    break;
                default:
                    cout << "Invalid choice for Player 1. Exiting the game.\n";
                    return 1;
            }
            break;
        case 3:
            switch(choice) {
                case 1:
                    players[0] = new TicTacToe5x5_Player<char>(playerXName, 'X');
                    break;
                default:
                    cout << "Invalid choice for Player 1. Exiting the game.\n";
                    return 1;
            }
            break;            
        case 6:
            switch(choice) {
                case 1:
                    players[0] = new MisereTicTacToe_Player<char>(playerXName, 'X');
                    break;
                default:
                    cout << "Invalid choice for Player 1. Exiting the game.\n";
                    return 1;
            }
            break;            
    }
    // Set up player 2
    cout << "Enter Player 2 name: ";
    cin >> player2Name;
    cout << "Choose Player 2 type:\n";
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cin >> choice;

    switch(gameChoice) {
        case 0:
            switch(choice) {
                case 1:
                    players[1] = new X_O_Player<char>(player2Name, 'O');
                    break;
                case 2:
                    players[1] = new X_O_Random_Player<char>('O');
                    break;
                case 3:
                    players[1] = new X_O_MinMax_Player<char>('O');
                    players[1]->setBoard(board);
                    break;
                default:
                    cout << "Invalid choice for Player 2. Exiting the game.\n";
                    return 1;
            }
            break;
        case 3:
            switch(choice) {
                case 1:
                    players[1] = new TicTacToe5x5_Player<char>(player2Name, 'O');
                    break;
                default:
                    cout << "Invalid choice for Player 2. Exiting the game.\n";
                    return 1;
            }
            break;
        case 6:
            switch(choice) {
                case 1:
                    players[1] = new MisereTicTacToe_Player<char>(player2Name, 'O');
                    break;
                default:
                    cout << "Invalid choice for Player 2. Exiting the game.\n";
                    return 1;
            }
            break;
    }

/*
#define B(s) board->update_board(B_i, B_j++, s);
#define BR B_j = 0; B_i++;
    int B_i = 0;
    int B_j = 0;
    B('O') B('X') B('O') B('X') B(0)   BR
    B('X') B('O') B('O') B('O') B(0)   BR
    B('O') B('X') B('X') B('X') B('O') BR
    B('X') B('O') B('X') B('O') B('X') BR
    B('O') B('X') B('O') B('X') B('O') BR
*/

    GameManager<char> game(board, players);
    game.run();

    // Clean up
    delete board;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }

    return 0;
}
