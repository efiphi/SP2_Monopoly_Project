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
    currentPlayer->move(totalSteps);

    // Check if the player passed the Start tile
    if (currentPlayer->getPosition() < initialPosition) {
        std::cout << "Player " << currentPlayer->getName() << " passed Start and collects $200!" << std::endl;
        currentPlayer->collectFromStart(200);
    }

    // Interact with the tile the player landed on
    auto tile = board.getTile(currentPlayer->getPosition());
    tile->onLand(currentPlayer);

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

void Game::addTile(std::shared_ptr<Tile> tile, const sf::Vector2f& position) {
    board.addTile(tile, position);
}

void Game::drawBoard(sf::RenderWindow &window) {
    sf::RectangleShape tile;
    const double tileSize = 61.5;
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
    const int cornerTileSize = 118;
    const int windowWidth = 800;
    const int windowHeight = 800;

    // Define tile positions (clockwise, starting from GO)
    std::vector<sf::Vector2f> tilePositions = {
        {750, 741}, {655, 741}, {600, 741}, {540, 741}, {475, 741}, {405, 741}, {330, 741}, {250, 741}, {200, 741}, {140, 741}, 
        {50, 741}, {50, 655}, {50, 600}, {50, 540}, {50, 475}, {50, 405}, {50, 330}, {50, 250}, {50, 200}, {50, 140}, 
        {50, 50}, {140, 50}, {200, 50}, {250, 50}, {330, 50}, {405, 50}, {475, 50}, {540, 50}, {600, 50}, {655, 50}, 
        {750, 50}, {750, 140}, {750, 200}, {750, 250}, {750, 330}, {750, 405}, {750, 475}, {750, 540}, {750, 600}, {750, 655}
    };

    // Group players by their tile position
    std::map<int, std::vector<std::shared_ptr<Player>>> playerGroups;
    for (const auto& player : players) {
        playerGroups[player->location].push_back(player);
    }

    // Now draw each player on the corresponding tile
    for (const auto& group : playerGroups) {
        int tileIndex = group.first;
        const auto& tilePosition = tilePositions[tileIndex];
        int offsetCounter = 0;  // To adjust position if there are multiple players on the same tile

        for (const auto& player : group.second) {
            sf::CircleShape playerCircle(10);
            playerCircle.setFillColor(player->color);
            
            sf::Vector2f adjustedPosition = tilePosition;
            
            // Adjust the position based on the tile's row or column
            if (tileIndex >= 0 && tileIndex <= 9) {  // Bottom row
                adjustedPosition.y -= 20 * offsetCounter;  // Move players upward on the bottom row
            } else if (tileIndex >= 10 && tileIndex <= 19) {  // Left column
                adjustedPosition.x -= 20 * offsetCounter;  // Move players to the left
            } else if (tileIndex >= 20 && tileIndex <= 29) {  // Top row
                adjustedPosition.y += 20 * offsetCounter;  // Move players downward on the top row
            } else if (tileIndex >= 30 && tileIndex <= 39) {  // Right column
                adjustedPosition.x += 20 * offsetCounter;  // Move players to the right
            }

            playerCircle.setPosition(adjustedPosition.x - 5, adjustedPosition.y - 5);  // Adjust for center
            window.draw(playerCircle);

            offsetCounter++;  // Increase offset for the next player on the same tile
        }
    }
}


void Game::initializeBoard() {
    // Bottom row (right to left)
    addTile(std::make_shared<StartTile>("Go"), {750, 741});                      // 0
    addTile(std::make_shared<StreetTile>("Mediterranean Ave", "Brown", 60, 2), {655, 741});  // 1
    addTile(std::make_shared<CommunityChestTile>("Community Chest"), {600, 741}); // 2
    addTile(std::make_shared<StreetTile>("Baltic Ave", "Brown", 60, 4), {540, 741});    // 3
    addTile(std::make_shared<TaxTile>("Income Tax"), {475, 741});                // 4
    addTile(std::make_shared<RailroadTile>("Reading Railroad"), {405, 741});     // 5
    addTile(std::make_shared<StreetTile>("Oriental Ave", "Light Blue", 100, 6), {330, 741}); // 6
    addTile(std::make_shared<ChanceTile>("Chance"), {250, 741});                 // 7
    addTile(std::make_shared<StreetTile>("Vermont Ave", "Light Blue", 100, 6), {200, 741}); // 8
    addTile(std::make_shared<StreetTile>("Connecticut Ave", "Light Blue", 120, 8), {140, 741}); // 9

    // Left column (bottom to top)
    addTile(std::make_shared<JailTile>("Jail"), {50, 741});                      // 10
    addTile(std::make_shared<StreetTile>("St. Charles Place", "Pink", 140, 10), {50, 655}); // 11
    addTile(std::make_shared<UtilityTile>("Electric Company"), {50, 600});       // 12
    addTile(std::make_shared<StreetTile>("States Ave", "Pink", 140, 10), {50, 540}); // 13
    addTile(std::make_shared<StreetTile>("Virginia Ave", "Pink", 160, 12), {50, 475}); // 14
    addTile(std::make_shared<RailroadTile>("Pennsylvania Railroad"), {50, 405}); // 15
    addTile(std::make_shared<StreetTile>("St. James Place", "Orange", 180, 14), {50, 330}); // 16
    addTile(std::make_shared<CommunityChestTile>("Community Chest"), {50, 250}); // 17
    addTile(std::make_shared<StreetTile>("Tennessee Ave", "Orange", 180, 14), {50, 200}); // 18
    addTile(std::make_shared<StreetTile>("New York Ave", "Orange", 200, 16), {50, 140}); // 19

    // Top row (left to right)
    addTile(std::make_shared<FreeParkingTile>("Free Parking"), {50, 50});        // 20
    addTile(std::make_shared<StreetTile>("Kentucky Ave", "Red", 220, 18), {140, 50}); // 21
    addTile(std::make_shared<ChanceTile>("Chance"), {200, 50});                  // 22
    addTile(std::make_shared<StreetTile>("Indiana Ave", "Red", 220, 18), {250, 50}); // 23
    addTile(std::make_shared<StreetTile>("Illinois Ave", "Red", 240, 20), {330, 50}); // 24
    addTile(std::make_shared<RailroadTile>("B&O Railroad"), {405, 50});          // 25
    addTile(std::make_shared<StreetTile>("Atlantic Ave", "Yellow", 260, 22), {475, 50}); // 26
    addTile(std::make_shared<StreetTile>("Ventnor Ave", "Yellow", 260, 22), {540, 50}); // 27
    addTile(std::make_shared<UtilityTile>("Water Works"), {600, 50});            // 28
    addTile(std::make_shared<StreetTile>("Marvin Gardens", "Yellow", 280, 24), {655, 50}); // 29

    // Right column (top to bottom)
    addTile(std::make_shared<GoToJailTile>("Go to Jail"), {750, 50});            // 30
    addTile(std::make_shared<StreetTile>("Pacific Ave", "Green", 300, 26), {750, 140}); // 31
    addTile(std::make_shared<StreetTile>("North Carolina Ave", "Green", 300, 26), {750, 200}); // 32
    addTile(std::make_shared<CommunityChestTile>("Community Chest"), {750, 250}); // 33
    addTile(std::make_shared<StreetTile>("Pennsylvania Ave", "Green", 320, 28), {750, 330}); // 34
    addTile(std::make_shared<RailroadTile>("Short Line"), {750, 405});           // 35
    addTile(std::make_shared<ChanceTile>("Chance"), {750, 475});                 // 36
    addTile(std::make_shared<StreetTile>("Park Place", "Blue", 350, 35), {750, 540}); // 37
    addTile(std::make_shared<TaxTile>("Luxury Tax"), {750, 600});                // 38
    addTile(std::make_shared<StreetTile>("Boardwalk", "Blue", 400, 50), {750, 655}); // 39
}
