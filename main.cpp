#include <iostream>
#include <memory>
#include "game.hpp"
#include "player.hpp"

int main() {
    std::cout << "Welcome to the Interactive Monopoly Game!\n";

    // Create players
    auto player1 = std::make_shared<Player>("Alice", 1500);
    auto player2 = std::make_shared<Player>("Bob", 1500);
    auto player3 = std::make_shared<Player>("Mark", 1500);
    Game game({player1, player2, player3});

    game.initializeBoard();  // Initialize the board with all tiles

    bool gameFinished = false;
    bool exitFlag = false;  // Global flag to track if the game should exit

    while (!gameFinished && !exitFlag) {
        // Main game loop: Iterate through players and offer choices
        auto currentPlayer = game.getCurrentPlayer();
        bool endTurn = false;

        std::cout << "\nIt's " << currentPlayer->getName() << "'s turn.\n";

        while (!endTurn && !exitFlag) {
            // Display options for the current player
            game.displayPlayerOptions();
            int choice;
            std::cin >> choice;

            // Handle the player's choice
            game.handlePlayerChoice(choice, endTurn, exitFlag);

            // Check if the player chose to exit the game
            if (exitFlag) {
                std::cout << "Exiting the game...\n";
                gameFinished = true;
                break;
            }

            // If a player is bankrupt or wins, end the game
            if (game.checkForWinner() || currentPlayer->isBankrupt()) {
                gameFinished = true;
                break;
            }
        }

        if (!exitFlag) {
            game.nextPlayer();  // Move to the next player's turn only if not exiting
        }
    }

    std::cout << "Game Over! Thanks for playing.\n";
    return 0;
}
