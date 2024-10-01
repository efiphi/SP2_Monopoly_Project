# Monopoly Game Implementation
## Introduction
This project implements a simplified version of the classic board game Monopoly. The game is developed using C++ and incorporates SFML for graphical representation. This project covers core Monopoly game mechanics, including property trading, building houses, paying rent, and handling special tiles such as Chance and Community Chest. The goal is to provide a rich gaming experience while also ensuring robust testing and code structure.

The project adheres to the instructions provided in this repository and follows the guidelines for efficient code structure and testing.

## Class Hierarchy
The project is structured into several classes, each representing different game components:

### Player
Represents a player in the game.

    Attributes:

      name: The player's name.
      money: Current balance of the player.
      properties: List of properties owned by the player.
      position: Current position on the board.
      inJail: Jail status of the player.
      getOutOfJailFreeCards: Number of "Get Out of Jail Free" cards owned.
      Methods:
      
      Player(const std::string& name, int startingMoney = 1500): Initializes a new player with a given name and starting money.
      move(int steps): Moves the player by a specified number of tiles.
      adjustMoney(int amount): Adjusts the player's balance by a specified amount.
      buyProperty(std::shared_ptr<Tile> property): Buys a property if the player has enough money.
      payRent(Player& owner, int rentAmount): Pays rent to another player.
      goToJail(): Sends the player to jail.
      releaseFromJail(): Releases the player from jail.
      isBankrupt(): Checks if the player is bankrupt.
    
### Tile
Represents a general tile on the board.

    Attributes:

      name: Name of the tile.
      tileType: Type of the tile (Street, Railroad, Utility, etc.).
      owner: Owner of the tile (if applicable).
      Methods:
      
      Tile(const std::string& name, const std::string& type): Initializes a tile with a name and type.
      getName(): Returns the name of the tile.
      isOccupied(): Checks if the tile is owned.
      setOwner(std::shared_ptr<Player> newOwner): Sets the owner of the tile.
      
### StreetTile
A specialized class derived from Tile representing a street in the game.

    Attributes:
  
      colorGroup: Color group of the street (e.g., Blue, Pink).
      basePrice: Base price of the street.
      baseRent: Base rent of the street.
      houseCount: Number of houses built on the street.
      hotelBuilt: Whether a hotel has been built on the street.
      Methods:
      
      StreetTile(const std::string& name, const std::string& color, int basePrice, int baseRent): Initializes a street tile with its properties.
      buildHouse(): Builds a house on the street.
      buildHotel(): Builds a hotel on the street.
      calculateRent(): Calculates the rent based on the number of houses or hotel present.
    
### RailroadTile
A class derived from Tile representing railroads in the game.

    Methods:
    RailroadTile(const std::string& name): Initializes a railroad tile.
    calculateRent(int railroadsOwned): Calculates rent based on the number of railroads owned by the player.
    
### UtilityTile
A class derived from Tile representing utilities in the game.

    Methods:
    UtilityTile(const std::string& name): Initializes a utility tile.
    calculateRent(int diceRoll, int utilitiesOwned): Calculates rent based on the dice roll and the number of utilities owned.
    
### Game
Manages the entire game state, including players, turns, and game rules.

       Attributes:

        players: List of all players.
        currentPlayerIndex: Index of the player whose turn it is.
        board: The game board, which holds all the tiles.
        doubleCount: To track consecutive doubles.
        dice: Dice object used for rolling.
        
    Methods:
        
        Game(const std::vector<std::shared_ptr<Player>>& playerList): Initializes the game with a list of players.
        playTurn(): Executes a single turn for the current player.
        checkForWinner(): Checks if any player has won the game.
        checkBankruptcy(): Removes bankrupt players from the game.
        getCurrentPlayer(): Returns the current player.
        getTile(int index): Returns a tile at a specific index.
        drawBoard(sf::RenderWindow& window): Draws the entire board.
        drawPlayers(sf::RenderWindow& window, const std::vector<std::shared_ptr<Player>>& players): Draws player tokens on the board.
        drawStar(sf::RenderWindow& window, const sf::Vector2f& position, int tileIndex, sf::Color color): Draws markers for tile occupation.
        addTile(std::shared_ptr<Tile> tile, const sf::Vector2f& position): Adds a new tile to the board.
        initializeBoard(): Initializes the board with all the required tiles.
        setDice(std::shared_ptr<Dice> customDice): Sets a custom dice (useful for testing).
        getDiceRoll(): Returns the most recent dice roll.
        isDouble(const std::pair<int, int>& diceRoll): Checks if a roll is a double.
    
### Board
Represents the entire game board.

    Attributes:
  
      tiles: A vector of all the tiles in the game.
      tileCount: Total number of tiles on the board.
      Methods:
      
      Board::getInstance(): Returns the singleton instance of the board.
      getTile(int index): Returns a tile at a specific index.
      removeTile(int index): Removes a tile from the board.
      getTileCount(): Returns the total number of tiles on the board.
      
### Cards
Represents a card in the game (either Chance or Community Chest).

    Methods:
    getDescription(): Returns a description of the card.
    execute(std::shared_ptr<Player>, Game&): Executes the effect of the card.
    
## Libraries Used
    <iostream>: For input and output operations.
    <vector>: For dynamic array handling.
    <map>: For storing and managing data types.
    <string>: For string manipulation.
    <memory>: For smart pointers.
    <SFML/Graphics.hpp>: For graphical rendering.
    <doctest.h>: For unit testing.

 ## Graphical Representation
When running the code, a window is displayed using SFML. The board is rendered, showing all the tiles and player tokens on their current positions. You can see the following visual elements:

    Player Locations: Displayed as colored tokens corresponding to each player.
    Property Ownership: Shown using markers for owned or free tiles.
    Special Tiles: Rendered distinctly for Chance, Community Chest, and other unique tiles.
    
## How to Run
Clone the repository:

bash

    git clone https://github.com/efiphi/SP2_Monopoly_Project.git

    
Navigate to the project directory:

bash

    cd SP2_Monopoly_Project
    
Build the project:

bash
    
      make  
      
Run the game:

bash

    ./monopoly_game
    
Run the tests:

bash

    make test
    ./test_game
    
## Authors
    Efi Phillips
    Contact: efiphi10@gmail.com
