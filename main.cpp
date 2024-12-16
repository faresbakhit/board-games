#include "BoardGame_Classes.h"
#include "Pyramic_XO.h"
#include "MisereTicTacToe.h"
#include "TicTacToe4x4.h"
#include "FourInARow.h"
#include "TicTacToe5x5.h"
#include "wordTic_tac_toe.h"
#include "NumericalTicTacToe.h"
#include "UltimateTicTacToe.h"
#include "XO.h"
#include <iostream>

using namespace std;

int main()
{
    int choice, gameChoice;
    Player<char> *players[2];
    Player<int> *numPlayers[2];
    Board<char> *board;
    Board<int> *numBoard;
    word_Board<char> *wordBoard = nullptr;

    cout << "Welcome to board-games.\n";

    while(true){

        cout << "0. 3x3 XO\n";
        cout << "1. Pyramic Tic-Tac-Toe\n";
        cout << "2. Four In Row\n";
        cout << "3. 5x5 Tic Tac Toe\n";
        cout << "4. Word Tic Tac Toe\n";
        cout << "5. Numerical Tic Tac Toe\n";
        cout << "6. Misere Tic Tac Toe\n";
        cout << "7. 4x4 Tic-Tac-Toe\n";
        cout << "8. Ultimate Tic-Tac-Toe\n";
        cout << "9. Exit\n";
        cout << "Enter board game type: ";
        cin >> gameChoice;

        if (gameChoice == 9) {
            cout << "Exiting the game !\n";
            break;
        }

        switch (gameChoice) {
        case 0:
            board = new XO_Board<char>();
            break;
        case 1:
            board = new Pyramic_XO_Board<char>();
            break;
        case 2:
            board = new FourInARowBoard<char>();
            break;
        case 3:
            board = new TicTacToe5x5_Board<char>();
            break;
        case 4:
            board = new word_Board<char>();
            break;
        case 5:
            numBoard = new NumericalTicTacToeBoard<int>();
            break;
        case 6:
            board = new MisereTicTacToe_Board<char>();
            break;
        case 7:
            board = new TicTacToe4x4_Board<char>();
            break;
        case 8:
            board = new UltimateTicTacToeBoard<char>();
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
            case 1:
                switch (playerType[playerIndex]) {
                case 1:
                    players[playerIndex] = new Pyramic_XO_Player<char>(
                        playerName[playerIndex], playerSymbol[playerIndex]);
                    break;
                case 2:
                    players[playerIndex]
                        = new Pyramic_XO_Random_Player<char>(playerSymbol[playerIndex]);
                    break;
                default:
                    cout << "Invalid choice for Player " << playerIndex
                        << ". Exiting the game.\n";
                    return 1;
                }
                break;
            case 2:
                switch (playerType[playerIndex]) {
                    case 1:
                        players[playerIndex]
                            = new FourInARowPlayer<char>(playerName[playerIndex], playerSymbol[playerIndex]);
                        break;
                    case 2:
                        players[playerIndex]
                            = new FourInARowRandomPlayer<char>(playerSymbol[playerIndex]);
                        break;
                    default:
                        cout << "Invalid choice for Player 1. Exiting the game.\n";
                        return 1;
                }
                break;
            case 3:
                switch (playerType[playerIndex]) {
                case 1:
                    players[playerIndex] = new TicTacToe5x5_Player<char>(
                        playerName[playerIndex], playerSymbol[playerIndex]);
                    break;
                case 3:
                    players[playerIndex]
                        = new TicTacToe5x5_AIPlayer<char>(playerSymbol[playerIndex]);
                    players[playerIndex]->setBoard(board);
                    break;
                default:
                    cout << "Invalid choice for Player " << playerIndex+1
                        << ". Exiting the game.\n";
                    return 1;
                }
                break;
            case 4:
                switch (playerType[playerIndex]) {
                case 1:
                    players[playerIndex] = new word_Player<char>(
                        playerName[playerIndex], playerSymbol[playerIndex]);
                    break;
                case 2:
                    players[playerIndex]
                        = new word_Random_Player<char>(playerSymbol[playerIndex]);
                    break;
                default:
                    cout << "Invalid choice for Player " << playerIndex
                        << ". Exiting the game.\n";
                    return 1;
                }
                break;
            case 5:
                switch(playerType[playerIndex]) {
                    case 1:
                        numPlayers[playerIndex]
                            = new NumericalTicTacToePlayer<int>(playerName[playerIndex],playerSymbol[playerIndex]);
                        numPlayers[playerIndex]->setBoard(numBoard);
                        break;
                    case 2:
                        numPlayers[playerIndex]
                            = new NumericalTicTacToeRandomPlayer<int>(playerSymbol[playerIndex]);
                        numPlayers[playerIndex]->setBoard(numBoard);
                        break;
                    default:
                            cout << "Invalid choice for Player 2. Exiting the game.\n";
                        return 1;
                }
                break;
            case 6:
                switch (playerType[playerIndex]) {
                case 1:
                    players[playerIndex] = new MisereTicTacToe_Player<char>(
                        playerName[playerIndex], playerSymbol[playerIndex]);
                    break;
                case 3:
                    players[playerIndex]
                        = new MisereTicTacToe_AIPlayer<char>(playerSymbol[playerIndex]);
                    players[playerIndex]->setBoard(board);
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
            case 8:
                switch (playerType[playerIndex]) {
                case 1:
                    players[playerIndex] = new UltimateTicTacToePlayer<char>(
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

        // Run the game
        if (gameChoice == 5) {
            GameManager<int> numGame(numBoard, numPlayers);
            numGame.run();
        } 
        else {
            GameManager<char> game(board, players);
            game.run();
        }

        // Cleanup
        if (gameChoice != 5) delete board;
        else delete numBoard;
        for (int i = 0; i < 2; i++) {
            if (gameChoice != 5) delete players[i];
            else delete numPlayers[i];
        }
    }
    return 0;    
}
