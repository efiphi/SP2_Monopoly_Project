#include "game.hpp"
#include <iostream>
#include <algorithm> // for std::remove_if

// Play a turn for the current player
void Game::playTurn() {
    if (players.empty()) return; // Safety check in case of no players

    std::shared_ptr<Player> currentPlayer = players[currentPlayerIndex];

    if (currentPlayer->isBankrupt()) {
        nextPlayer();  // Skip this player if bankrupt
        return;
    }

    // Roll the dice and store the result
    auto diceRoll = dice->roll();
    int totalSteps = dice->total(diceRoll);

    std::cout << "Player " << currentPlayer->getName() << " rolled " << diceRoll.first << " and " << diceRoll.second << std::endl;

    // Check for doubles
    if (dice->isDouble(diceRoll)) {
        doubleCount++;
        if (doubleCount == 3) {
            std::cout << "Three doubles in a row! Player " << currentPlayer->getName() << " goes to jail." << std::endl;
            currentPlayer->goToJail();
            doubleCount = 0;
            nextPlayer();
            return;
        }
    } else {
        doubleCount = 0;  // Reset the double count if no doubles were rolled
    }

    // Track player's initial position
    int initialPosition = currentPlayer->getPosition();

    if (board.getTileCount() == 0) {
        std::cerr << "Error: The board has no tiles!" << std::endl;
        return;
    }

    if (currentPlayer == nullptr) {
        std::cerr << "Error: Current player is null!" << std::endl;
        return;
    }

    // Move the player by the total steps rolled
    currentPlayer->move(totalSteps, board.getTileCount());

    // Check if the player passed the Start tile (board wraps around)
    if (currentPlayer->getPosition() < initialPosition) {
        std::cout << "Player " << currentPlayer->getName() << " passed Start and collects $200!" << std::endl;
        currentPlayer->collectFromStart(200);
    }

    // Interact with the tile the player landed on
    auto tile = board.getTile(currentPlayer->getPosition());
    if (tile == nullptr) {
        std::cerr << "Error: Invalid tile!" << std::endl;
        return;
    }
    tile->onLand(*currentPlayer);

    // Check if the player is bankrupt after paying rent or buying a property
    if (currentPlayer->isBankrupt()) {
        std::cout << currentPlayer->getName() << " has gone bankrupt!" << std::endl;
        checkBankruptcy();
        if (checkForWinner()) return;
    }

    // If the player rolled doubles, they get another turn
    if (dice->isDouble(diceRoll)) {
        std::cout << "Player " << currentPlayer->getName() << " gets another turn for rolling doubles!" << std::endl;
        playTurn();  // Recursively handle another turn
    } else {
        nextPlayer();  // Move to the next player
    }

    if (checkForWinner()) {
        return;  // End the game if there's a winner
    }
}



// Proceed to the next player
void Game::nextPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

// Check if a player has won the game
bool Game::checkForWinner() {
    if (players.size() == 1) {
        std::cout << "Player " << players[0]->getName() << " has won the game!" << std::endl;
        return true;  // Game over, one player left
    }
    return false;  // No winner yet
}

// Start the game
void Game::start() {
    while (true) {
        playTurn();  // Continuously play turns
        if (checkForWinner()) break;  // End the game if there's a winner
    }
}

void Game::addTile(std::shared_ptr<Tile> tile) {
    board.addTile(tile);
}
