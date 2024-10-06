#ifndef GAME_HPP
#define GAME_HPP

#include "board.hpp"
#include "dice.hpp"
#include "player.hpp"
#include "specialTiles.hpp"
#include <memory>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

class Game {
private:
    Board& board;
    int doubleCount; // To track consecutive doubles
    std::shared_ptr<Dice> dice;  // Use shared_ptr for Dice, allowing MockDice to be injected
    std::vector<std::shared_ptr<Player>> players; // Use shared_ptr for players
    int currentPlayerIndex;
    std::pair<int, int> lastDiceRoll;

public:
    // Constructor
   Game(const std::vector<std::shared_ptr<Player>>& playerList)
    : board(Board::getInstance()), players(playerList), currentPlayerIndex(0), doubleCount(0), dice(std::make_shared<Dice>()) {
    // Available player colors (add more as needed)
    std::vector<sf::Color> playerColors = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow};

    // Assuming players are initialized as follows:
    for (size_t i = 0; i < players.size(); ++i) {
        players[i]->setColor(playerColors[i % playerColors.size()]);
    }

    }

    // Play a turn for the current player
    void playTurn();

    // Proceed to the next player
    void nextPlayer();

    // Check if a player has won the game
    bool checkForWinner();

    // Add a tile to the board
    void addTile(std::shared_ptr<Tile> tile, const sf::Vector2f& position);

    // Start the game
    void start();

    Board& getBoard() {
        return board;
    }

    // Check and remove bankrupt players
    void checkBankruptcy() {
        players.erase(std::remove_if(players.begin(), players.end(),
            [](const std::shared_ptr<Player>& player) { return player->isBankrupt(); }),
            players.end());
    }

    std::shared_ptr<Player> getCurrentPlayer() const {
        return players[currentPlayerIndex];
    }

    std::shared_ptr<Tile> getTile(int index) const {
        return board.getTile(index);  // Delegate to the Board class's getTile
    }
    

    sf::Vector2f getTilePosition(int tileIndex, double tileSize, int cornerTileSize);
    void displayBoard();
    void drawPlayers(sf::RenderWindow &window, const std::vector<std::shared_ptr<Player>>& players);
    void drawBoard(sf::RenderWindow& window);
    void drawStar(sf::RenderWindow &window, const sf::Vector2f &position, int tileIndex, sf::Color color);
    void initializeBoard();

    // Use shared_ptr to set the Dice
    void setDice(std::shared_ptr<Dice> customDice) {
    this->dice = customDice;
    }

    std::shared_ptr<Dice> rollDice() {
    return std::make_shared<Dice>();
    }



    std::pair<int, int> getDiceRoll() const { return lastDiceRoll; }  // Expose last dice roll
    bool isDouble(const std::pair<int, int>& diceRoll) const { return dice->isDouble(diceRoll); }  // Check for double

    void displayPlayerOptions() const;
    void handlePlayerChoice(int choice, bool &endTurn, bool &exitFlag);
    void displayPlayerDetails(const std::shared_ptr<Player>& player) const;
    void handlePropertyPurchase(const std::shared_ptr<Player>& player, bool isHouse);
    
};

#endif // GAME_HPP
