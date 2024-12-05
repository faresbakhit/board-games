#include "BoardGame_Classes.h"
#include "MisereTicTacToe.h"
#include "TicTacToe4x4.h"
#include "TicTacToe5x5.h"
#include "XO.h"
#include <iostream>

using namespace std;

int main()
{
    int choice, gameChoice;
    Player<char>* players[2];
    Board<char>* board;

    cout << "Welcome to board-games.\n";
    cout << "0. 3x3 XO\n";
    cout << "3. 5x5 Tic Tac Toe\n";
    cout << "6. Misere Tic Tac Toe\n";
    cout << "7. 4x4 Tic-Tac-Toe\n";
    cout << "Enter board game type: ";
    cin >> gameChoice;

    switch (gameChoice) {
    case 0:
        board = new XO_Board<char>();
        break;
    case 3:
        board = new TicTacToe5x5_Board<char>();
        break;
    case 6:
        board = new MisereTicTacToe_Board<char>();
        break;
    case 7:
        board = new TicTacToe4x4_Board<char>();
        break;
    default:
        cout << "Invalid choice for board game type. Exiting the game.\n";
        return 1;
    }

    string playerName[2];
    char playerSymbol[2] = { 'X', 'O' };
    int playerType[2];

    cout << "Enter Player 1 name: ";
    cin >> playerName[0];
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cout << "Choose Player 1 type: ";
    cin >> playerType[0];

    cout << "Enter Player 2 name: ";
    cin >> playerName[1];
    cout << "1. Human\n";
    cout << "2. Random Computer\n";
    cout << "3. Smart Computer (AI)\n";
    cout << "Choose Player 2 type: ";
    cin >> playerType[1];

    for (size_t playerIndex : { 0, 1 }) {
        switch (gameChoice) {
        case 0:
            switch (playerType[playerIndex]) {
            case 1:
                players[playerIndex] = new XO_Player<char>(
                    playerName[playerIndex], playerSymbol[playerIndex]);
                break;
            case 2:
                players[playerIndex]
                    = new XO_Random_Player<char>(playerSymbol[playerIndex]);
                break;
            case 3:
                players[playerIndex]
                    = new XO_MinMax_Player<char>(playerSymbol[playerIndex]);
                players[playerIndex]->setBoard(board);
                break;
            default:
                cout << "Invalid choice for Player " << playerIndex
                     << ". Exiting the game.\n";
                return 1;
            }
            break;
        case 3:
            switch (playerType[playerIndex]) {
            case 1:
                players[playerIndex] = new TicTacToe5x5_Player<char>(
                    playerName[playerIndex], playerSymbol[playerIndex]);
                break;
            default:
                cout << "Invalid choice for Player " << playerIndex
                     << ". Exiting the game.\n";
                return 1;
            }
            break;
        case 6:
            switch (playerType[playerIndex]) {
            case 1:
                players[playerIndex] = new MisereTicTacToe_Player<char>(
                    playerName[playerIndex], playerSymbol[playerIndex]);
                break;
            default:
                cout << "Invalid choice for Player " << playerIndex
                     << ". Exiting the game.\n";
                return 1;
            }
            break;
        case 7:
            switch (playerType[playerIndex]) {
            case 1:
                players[playerIndex] = new TicTacToe4x4_Player<char>(
                    playerName[playerIndex], playerSymbol[playerIndex]);
                break;
            default:
                cout << "Invalid choice for Player " << playerIndex
                     << ". Exiting the game.\n";
                return 1;
            }
            break;
        }
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
