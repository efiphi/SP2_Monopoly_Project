#include "game.hpp"
#include <iostream>
#include <algorithm> 

// Play a turn for the current player
void Game::playTurn() {
    if (players.empty()) return; // Safety check in case of no players

    std::shared_ptr<Player> currentPlayer = players[currentPlayerIndex];

    if (currentPlayer->isBankrupt()) {
        nextPlayer();  // Skip this player if bankrupt
        return;
    }

    auto diceRoll = dice->roll();
    int totalSteps = dice->total(diceRoll);

    // Log the dice roll result
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

    // Move the player and update the position
    int initialPosition = currentPlayer->getPosition();
    currentPlayer->move(totalSteps, board.getTileCount());

    // Check if the player passed the Start tile
    if (currentPlayer->getPosition() < initialPosition) {
        std::cout << "Player " << currentPlayer->getName() << " passed Start and collects $200!" << std::endl;
        currentPlayer->collectFromStart(200);
    }

    // Interact with the tile the player landed on
    auto tile = board.getTile(currentPlayer->getPosition());
    tile->onLand(*currentPlayer);

    if (currentPlayer->isBankrupt()) {
        std::cout << currentPlayer->getName() << " has gone bankrupt!" << std::endl;
        checkBankruptcy();
        if (checkForWinner()) return;
    }

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

void Game::drawBoard(sf::RenderWindow &window) {
    sf::RectangleShape tile;
    const double tileSize = 61.5;
    const double tileWidth = 100;
    const int cornerTileSize = 118;
    const int windowWidth = 800;
    const int windowHeight = 800;
    
    // Adjustments to the outline
    const int outlineThickness = 2;

    // Corner tiles with larger size
    sf::RectangleShape cornerTile(sf::Vector2f(cornerTileSize, cornerTileSize));
    cornerTile.setFillColor(sf::Color::White);
    cornerTile.setOutlineColor(sf::Color::Black);
    cornerTile.setOutlineThickness(outlineThickness);

    // Top-left corner
    cornerTile.setPosition(outlineThickness, outlineThickness);
    window.draw(cornerTile);

    // Top-right corner
    cornerTile.setPosition(windowWidth - cornerTileSize - outlineThickness, outlineThickness);
    window.draw(cornerTile);

    // Bottom-left corner
    cornerTile.setPosition(outlineThickness, windowHeight - cornerTileSize - outlineThickness);
    window.draw(cornerTile);

    // Bottom-right corner
    cornerTile.setPosition(windowWidth - cornerTileSize - outlineThickness, windowHeight - cornerTileSize - outlineThickness);
    window.draw(cornerTile);

    // Regular tiles
    sf::RectangleShape verticalTile(sf::Vector2f(cornerTileSize, tileSize));
    verticalTile.setFillColor(sf::Color::White);
    verticalTile.setOutlineColor(sf::Color::Black);
    verticalTile.setOutlineThickness(outlineThickness);

     // Left column (between corners)
    for (int i = 1; i < 10; ++i) {
        verticalTile.setPosition(outlineThickness, cornerTileSize + 2.5 + (i - 1) * tileSize + outlineThickness);
        window.draw(verticalTile);
    }

    // Right column (between corners)
    for (int i = 1; i < 10; ++i) {
        verticalTile.setPosition(windowWidth - cornerTileSize - outlineThickness, cornerTileSize + 2.5 + (i - 1) * tileSize + outlineThickness);
        window.draw(verticalTile);
    }

    sf::RectangleShape horizontalTile(sf::Vector2f(tileSize, cornerTileSize));
    horizontalTile.setFillColor(sf::Color::White);
    horizontalTile.setOutlineColor(sf::Color::Black);
    horizontalTile.setOutlineThickness(outlineThickness);

    // Top row (between corners)
    for (int i = 1; i < 10; ++i) {
        horizontalTile.setPosition(cornerTileSize + 3 + (i - 1) * tileSize + outlineThickness, outlineThickness);
        window.draw(horizontalTile);
    }

    // Bottom row (between corners)
    for (int i = 1; i < 10; ++i) {
        horizontalTile.setPosition(cornerTileSize + 3 + (i - 1) * tileSize + outlineThickness, windowHeight - cornerTileSize - outlineThickness);
        window.draw(horizontalTile);
    }

   
}



void Game::initializeBoard() {
    // Bottom row (left to right)
    addTile(std::make_shared<StartTile>("Go"));
    addTile(std::make_shared<StreetTile>("Mediterranean Ave", "Brown", 60, 2));
    addTile(std::make_shared<CommunityChestTile>("Community Chest"));
    addTile(std::make_shared<StreetTile>("Baltic Ave", "Brown", 60, 4));
    addTile(std::make_shared<TaxTile>("Income Tax"));
    addTile(std::make_shared<RailroadTile>("Reading Railroad"));
    addTile(std::make_shared<StreetTile>("Oriental Ave", "Light Blue", 100, 6));
    addTile(std::make_shared<ChanceTile>("Chance"));
    addTile(std::make_shared<StreetTile>("Vermont Ave", "Light Blue", 100, 6));
    addTile(std::make_shared<StreetTile>("Connecticut Ave", "Light Blue", 120, 8));

    // Right column (top to bottom)
    addTile(std::make_shared<JailTile>("Jail"));
    addTile(std::make_shared<StreetTile>("St. Charles Place", "Pink", 140, 10));
    addTile(std::make_shared<UtilityTile>("Electric Company"));
    addTile(std::make_shared<StreetTile>("States Ave", "Pink", 140, 10));
    addTile(std::make_shared<StreetTile>("Virginia Ave", "Pink", 160, 12));
    addTile(std::make_shared<RailroadTile>("Pennsylvania Railroad"));
    addTile(std::make_shared<StreetTile>("St. James Place", "Orange", 180, 14));
    addTile(std::make_shared<CommunityChestTile>("Community Chest"));
    addTile(std::make_shared<StreetTile>("Tennessee Ave", "Orange", 180, 14));
    addTile(std::make_shared<StreetTile>("New York Ave", "Orange", 200, 16));

    // Top row (right to left)
    addTile(std::make_shared<FreeParkingTile>("Free Parking"));
    addTile(std::make_shared<StreetTile>("Kentucky Ave", "Red", 220, 18));
    addTile(std::make_shared<ChanceTile>("Chance"));
    addTile(std::make_shared<StreetTile>("Indiana Ave", "Red", 220, 18));
    addTile(std::make_shared<StreetTile>("Illinois Ave", "Red", 240, 20));
    addTile(std::make_shared<RailroadTile>("B&O Railroad"));
    addTile(std::make_shared<StreetTile>("Atlantic Ave", "Yellow", 260, 22));
    addTile(std::make_shared<StreetTile>("Ventnor Ave", "Yellow", 260, 22));
    addTile(std::make_shared<UtilityTile>("Water Works"));
    addTile(std::make_shared<StreetTile>("Marvin Gardens", "Yellow", 280, 24));

    // Left column (bottom to top)
    addTile(std::make_shared<GoToJailTile>("Go to Jail"));
    addTile(std::make_shared<StreetTile>("Pacific Ave", "Green", 300, 26));
    addTile(std::make_shared<StreetTile>("North Carolina Ave", "Green", 300, 26));
    addTile(std::make_shared<CommunityChestTile>("Community Chest"));
    addTile(std::make_shared<StreetTile>("Pennsylvania Ave", "Green", 320, 28));
    addTile(std::make_shared<RailroadTile>("Short Line"));
    addTile(std::make_shared<ChanceTile>("Chance"));
    addTile(std::make_shared<StreetTile>("Park Place", "Blue", 350, 35));
    addTile(std::make_shared<TaxTile>("Luxury Tax"));
    addTile(std::make_shared<StreetTile>("Boardwalk", "Blue", 400, 50));
}
