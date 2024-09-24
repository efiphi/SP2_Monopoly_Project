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

    // Load the Monopoly image
    sf::Texture monopolyTexture;
    if (!monopolyTexture.loadFromFile("monopoly.jpg")) {
        std::cout << "Error loading Monopoly image" << std::endl;
        return;
    }

    // Create a sprite for the Monopoly image
    sf::Sprite monopolyImage;
    monopolyImage.setTexture(monopolyTexture);
    
    // Scale the image to fit the entire 800x800 window
    monopolyImage.setScale(
        static_cast<float>(windowWidth) / monopolyTexture.getSize().x,
        static_cast<float>(windowHeight) / monopolyTexture.getSize().y
    );

    
   

    // Corner tiles with larger size
    sf::RectangleShape cornerTile(sf::Vector2f(cornerTileSize, cornerTileSize));
    cornerTile.setFillColor(sf::Color::Transparent);  // Make the corners transparent to show the image underneath
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
    verticalTile.setFillColor(sf::Color::Transparent);  // Tiles transparent to show the image
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
    horizontalTile.setFillColor(sf::Color::Transparent);  // Make transparent
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

     window.draw(monopolyImage);

      for (const auto& player : players) {
        sf::CircleShape playerMarker(10);  // Radius of the player marker
        playerMarker.setFillColor(player->color);
        
        // Get the position of the player's current tile
        sf::Vector2f tilePosition = getTilePosition(player->location, tileSize, cornerTileSize);
        
        // Offset the player position to be at the bottom-center of the tile
        playerMarker.setPosition(tilePosition.x + tileSize / 2 - 10, tilePosition.y + tileSize - 20);
        window.draw(playerMarker);
    }

}

sf::Vector2f Game::getTilePosition(int tileIndex, double tileSize, int cornerTileSize) {
    if (tileIndex < 10) {
        // Bottom row (going right)
        return sf::Vector2f(cornerTileSize + tileIndex * tileSize, 800 - cornerTileSize);
    } else if (tileIndex < 20) {
        // Left column (going up)
        return sf::Vector2f(0, cornerTileSize + (tileIndex - 10) * tileSize);
    } else if (tileIndex < 30) {
        // Top row (going left)
        return sf::Vector2f(800 - cornerTileSize - (tileIndex - 20) * tileSize, 0);
    } else {
        // Right column (going down)
        return sf::Vector2f(800 - cornerTileSize, cornerTileSize + (tileIndex - 30) * tileSize);
    }
}



void Game::drawPlayers(sf::RenderWindow &window, const std::vector<std::shared_ptr<Player>>& players) {
    const double tileSize = 61.5;
    const double tileWidth = 100;
    const int cornerTileSize = 118;
    const int windowWidth = 800;
    const int windowHeight = 800;
    // Define tile positions (clockwise, starting from GO)
    std::vector<sf::Vector2f> tilePositions = {
       // Update the player position mapping to start from "GO" and move clockwise
    {750, windowHeight - cornerTileSize / 2},   // 0
    {655, windowHeight - cornerTileSize / 2},   // 1
    {600, windowHeight - cornerTileSize / 2},   // 2
    {540, windowHeight - cornerTileSize / 2},   // 3
    {475, windowHeight - cornerTileSize / 2},   // 4
    {405, windowHeight - cornerTileSize / 2},   // 5
    {330, windowHeight - cornerTileSize / 2},   // 6
    {250, windowHeight - cornerTileSize / 2},   // 7
    {200, windowHeight - cornerTileSize / 2},   // 8
    {140, windowHeight - cornerTileSize / 2},   // 9
    {50, windowHeight - cornerTileSize / 2},    // 10
    {cornerTileSize / 2, 655},       // 11
    {cornerTileSize / 2, 600},       // 12
    {cornerTileSize / 2, 540},       // 13
    {cornerTileSize / 2, 475},       // 14
    {cornerTileSize / 2, 405},       // 15
    {cornerTileSize / 2, 330},       // 16
    {cornerTileSize / 2, 250},       // 17
    {cornerTileSize / 2, 200},       // 18
    {cornerTileSize / 2, 140},       // 19
    {cornerTileSize / 2, 50},        // 20
    {140, cornerTileSize / 2},       // 21
    {200, cornerTileSize / 2},       // 22
    {250, cornerTileSize / 2},       // 23
    {330, cornerTileSize / 2},       // 24
    {405, cornerTileSize / 2},       // 25
    {475, cornerTileSize / 2},       // 26
    {540, cornerTileSize / 2},       // 27
    {600, cornerTileSize / 2},       // 28
    {655, cornerTileSize / 2},       // 29
    {750, cornerTileSize / 2},       // 30
    {windowHeight - cornerTileSize / 2, 140},   // 31
    {windowHeight - cornerTileSize / 2, 200},   // 32
    {windowHeight - cornerTileSize / 2, 250},   // 33
    {windowHeight - cornerTileSize / 2, 330},   // 34
    {windowHeight - cornerTileSize / 2, 405},   // 35
    {windowHeight - cornerTileSize / 2, 475},   // 36
    {windowHeight - cornerTileSize / 2, 540},   // 37
    {windowHeight - cornerTileSize / 2, 600},   // 38
    {windowHeight - cornerTileSize / 2, 655}    // 39


    };

    // Now draw each player on the corresponding tile
    for (const auto& player : players) {
        sf::CircleShape playerCircle(10);
        playerCircle.setFillColor(player->color);
        int playerTile = player->location;  // Get the player's current location
        playerCircle.setPosition(tilePositions[playerTile].x - 5, tilePositions[playerTile].y - 5);  // Adjust for center

        window.draw(playerCircle);
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
