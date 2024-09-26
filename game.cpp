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

    // Roll dice, or use mocked dice if already set
    if (!dice) {
        dice = std::make_shared<Dice>(); // Reset to random dice if not set externally
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
            dice.reset();  // Reset dice for next turn
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

    // Handle bankruptcy after landing on a tile
    if (currentPlayer->isBankrupt()) {
        std::cout << currentPlayer->getName() << " has gone bankrupt!" << std::endl;
        checkBankruptcy();
        if (checkForWinner()) return;
    }

    // Handle doubles for extra turn
    if (dice->isDouble(diceRoll)) {
        std::cout << "Player " << currentPlayer->getName() << " gets another turn for rolling doubles!" << std::endl;
        dice.reset();  // Reset to random dice after the turn
        playTurn();  // Recursively handle another turn
    } else {
        dice.reset();  // Reset dice for the next player's turn
        nextPlayer();  // Move to the next player
    }

    // Check for game winner at the end of the turn
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

void Game::drawStar(sf::RenderWindow &window, const sf::Vector2f &position, int tileIndex, sf::Color color) {
    // Specify which tile indices correspond to special tiles where no stars should be drawn
    std::vector<int> noStarTiles = {0, 2, 7, 10, 17, 20, 22, 30, 33, 36};  // Indices for Go, Chance, Community Chest, Jail, etc.

    // Check if the current tile index is one of the special tiles
    if (std::find(noStarTiles.begin(), noStarTiles.end(), tileIndex) != noStarTiles.end()) {
        return;  // Do nothing if it's a special tile
    }

    // Create a star shape and set its color
    sf::CircleShape star(8, 5);  // Create a star-shaped polygon
    star.setFillColor(color);    // Set the star color

    // Adjust the position of the star based on the tile row/column
    sf::Vector2f adjustedPosition = position;
    if (tileIndex >= 0 && tileIndex <= 9) {  // Bottom row
        adjustedPosition.x -= 10;
        adjustedPosition.y -= 40;
    } else if (tileIndex >= 10 && tileIndex <= 19) {  // Left column
        adjustedPosition.x += 35;
        adjustedPosition.y += 5;
    } else if (tileIndex >= 20 && tileIndex <= 29) {  // Top row
        adjustedPosition.x -= 10;
        adjustedPosition.y += 35;
    } else if (tileIndex >= 30 && tileIndex <= 39) {  // Right column
        adjustedPosition.x -= 50;
        adjustedPosition.y -= 15;
    }

    // Set the adjusted position for the star and draw it
    star.setPosition(adjustedPosition.x, adjustedPosition.y);
    window.draw(star);
}

void Game::drawBoard(sf::RenderWindow &window) {
    sf::RectangleShape tile;
    
    const int windowWidth = 800;
    const int windowHeight = 800;
    
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

    window.draw(monopolyImage);  // Draw the background Monopoly image

    // Define tile positions
    std::vector<sf::Vector2f> tilePositions = {
        {750, 741}, {655, 741}, {600, 741}, {540, 741}, {475, 741}, {405, 741}, {330, 741}, {250, 741}, {200, 741}, {140, 741}, 
        {50, 741}, {50, 655}, {50, 600}, {50, 540}, {50, 475}, {50, 405}, {50, 330}, {50, 250}, {50, 200}, {50, 140}, 
        {50, 50}, {140, 50}, {200, 50}, {250, 50}, {330, 50}, {405, 50}, {475, 50}, {540, 50}, {600, 50}, {655, 50}, 
        {750, 50}, {750, 140}, {750, 200}, {750, 250}, {750, 330}, {750, 405}, {750, 475}, {750, 540}, {750, 600}, {750, 655}
    };

    // Draw each tile's star, representing ownership
    for (int i = 0; i < board.getTileCount(); ++i) {
        auto tile = board.getTile(i);

        // Determine if the tile is occupied and get the owner's color
        sf::Color starColor = tile->isOccupied() ? tile->getOwner()->color : sf::Color::White;

        // Draw star on the top-right corner
        drawStar(window, tilePositions[i], i, starColor);
    }

    // Draw players on the board
    drawPlayers(window, players);
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

    Board& board = Board::getInstance();
    // Bottom row (right to left)
    board.addTile(std::make_shared<StartTile>("Go"), {750, 741});                      // 0
    board.addTile(std::make_shared<StreetTile>("Mediterranean Ave", "Brown", 60, 2), {655, 741});  // 1
    board.addTile(std::make_shared<CommunityChestTile>("Community Chest"), {600, 741}); // 2
    board.addTile(std::make_shared<StreetTile>("Baltic Ave", "Brown", 60, 4), {540, 741});    // 3
    board.addTile(std::make_shared<TaxTile>("Income Tax"), {475, 741});                // 4
    board.addTile(std::make_shared<RailroadTile>("Reading Railroad"), {405, 741});     // 5
    board.addTile(std::make_shared<StreetTile>("Oriental Ave", "Light Blue", 100, 6), {330, 741}); // 6
    board.addTile(std::make_shared<ChanceTile>("Chance"), {250, 741});                 // 7
    board.addTile(std::make_shared<StreetTile>("Vermont Ave", "Light Blue", 100, 6), {200, 741}); // 8
    board.addTile(std::make_shared<StreetTile>("Connecticut Ave", "Light Blue", 120, 8), {140, 741}); // 9

    // Left column (bottom to top)
    board.addTile(std::make_shared<JailTile>("Jail"), {50, 741});                      // 10
    board.addTile(std::make_shared<StreetTile>("St. Charles Place", "Pink", 140, 10), {50, 655}); // 11
    board.addTile(std::make_shared<UtilityTile>("Electric Company"), {50, 600});       // 12
    board.addTile(std::make_shared<StreetTile>("States Ave", "Pink", 140, 10), {50, 540}); // 13
    board.addTile(std::make_shared<StreetTile>("Virginia Ave", "Pink", 160, 12), {50, 475}); // 14
    board.addTile(std::make_shared<RailroadTile>("Pennsylvania Railroad"), {50, 405}); // 15
    board.addTile(std::make_shared<StreetTile>("St. James Place", "Orange", 180, 14), {50, 330}); // 16
    board.addTile(std::make_shared<CommunityChestTile>("Community Chest"), {50, 250}); // 17
    board.addTile(std::make_shared<StreetTile>("Tennessee Ave", "Orange", 180, 14), {50, 200}); // 18
    board.addTile(std::make_shared<StreetTile>("New York Ave", "Orange", 200, 16), {50, 140}); // 19

    // Top row (left to right)
    board.addTile(std::make_shared<FreeParkingTile>("Free Parking"), {50, 50});        // 20
    board.addTile(std::make_shared<StreetTile>("Kentucky Ave", "Red", 220, 18), {140, 50}); // 21
    board.addTile(std::make_shared<ChanceTile>("Chance"), {200, 50});                  // 22
    board.addTile(std::make_shared<StreetTile>("Indiana Ave", "Red", 220, 18), {250, 50}); // 23
    board.addTile(std::make_shared<StreetTile>("Illinois Ave", "Red", 240, 20), {330, 50}); // 24
    board.addTile(std::make_shared<RailroadTile>("B&O Railroad"), {405, 50});          // 25
    board.addTile(std::make_shared<StreetTile>("Atlantic Ave", "Yellow", 260, 22), {475, 50}); // 26
    board.addTile(std::make_shared<StreetTile>("Ventnor Ave", "Yellow", 260, 22), {540, 50}); // 27
    board.addTile(std::make_shared<UtilityTile>("Water Works"), {600, 50});            // 28
    board.addTile(std::make_shared<StreetTile>("Marvin Gardens", "Yellow", 280, 24), {655, 50}); // 29

    // Right column (top to bottom)
    board.addTile(std::make_shared<GoToJailTile>("Go to Jail"), {750, 50});            // 30
    board.addTile(std::make_shared<StreetTile>("Pacific Ave", "Green", 300, 26), {750, 140}); // 31
    board.addTile(std::make_shared<StreetTile>("North Carolina Ave", "Green", 300, 26), {750, 200}); // 32
    board.addTile(std::make_shared<CommunityChestTile>("Community Chest"), {750, 250}); // 33
    board.addTile(std::make_shared<StreetTile>("Pennsylvania Ave", "Green", 320, 28), {750, 330}); // 34
    board.addTile(std::make_shared<RailroadTile>("Short Line"), {750, 405});           // 35
    board.addTile(std::make_shared<ChanceTile>("Chance"), {750, 475});                 // 36
    board.addTile(std::make_shared<StreetTile>("Park Place", "Blue", 350, 35), {750, 540}); // 37
    board.addTile(std::make_shared<TaxTile>("Luxury Tax"), {750, 600});                // 38
    board.addTile(std::make_shared<StreetTile>("Boardwalk", "Blue", 400, 50), {750, 655}); // 39
}
