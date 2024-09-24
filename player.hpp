#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "tile.hpp"
#include "streetTile.hpp"
#include "railroadTile.hpp"

// Forward declaration of Tile class to avoid circular dependency
class Tile;

class Player : public std::enable_shared_from_this<Player> {
private:
    std::string name;                        // Player's name
    int money;                               // Player's current balance
    int position;                            // Player's current position on the board (index of the tile)
    bool inJail;                             // Whether the player is in jail
    int jailTurns;                           // Number of turns the player has spent in jail
    std::vector<std::shared_ptr<Tile>> ownedProperties;  // Properties owned by the player
    int lastDiceRoll;                        // Last dice roll result
    int numberOfUtilities;  
    

public:

    int location;
    sf::Color color;

    // Constructor
    Player(const std::string& name, int startingMoney = 1500)
        : name(name), money(startingMoney), position(0), inJail(false), jailTurns(0), lastDiceRoll(0), numberOfUtilities(0) {}
    
    Player(sf::Color c, int startLocation = 0)
        : color(c), location(startLocation) {}
    
    // Get player's name
    std::string getName() const { return name; }

    // Get player's current money
    int getMoney() const { return money; }

    // Adjust player's money
    void adjustMoney(int amount) { money += amount; }

    // Check if the player is bankrupt
    bool isBankrupt() const {
     return money == 0 && ownedProperties.empty();
    }

    void move(int steps) {
        location = (location + steps) % 40; // Move forward, loop back if passing 'GO'
    }



    // Get player's current position on the board
    int getPosition() const { return position; }

    // Move the player by a given number of positions
    void move(int steps, int boardSize) {
        position = (position + steps) % boardSize;  // Wrap around if they pass the start tile
    }

    // Equality operator
    bool operator==(const Player& other) const {
        return this == &other;  // Compare by memory address
    }

    bool operator!=(const Player& other) const {
        return !(*this == other);
    }

    // Jail management
    bool isInJail() const { return inJail; }
    void goToJail() { inJail = true; position = 10; jailTurns = 0; }
    void releaseFromJail() { inJail = false; }
    void handleJailTurn() {
        jailTurns++;
        if (jailTurns >= 3) {
            adjustMoney(-50);  // Pay to leave jail after 3 turns
            releaseFromJail();
        }
    }

    // Track dice rolls
    void setLastDiceRoll(int roll) { lastDiceRoll = roll; }
    int getLastDiceRoll() const { return lastDiceRoll; }

    // Collect money from passing the start tile
    void collectFromStart(int amount) {
        adjustMoney(amount);
    }

    // Get the number of utilities owned by the player
    int getNumberOfUtilities() const { return numberOfUtilities; }
    void incrementUtilitiesOwned() { ++numberOfUtilities; }

    // Buy property and manage ownership
    void buyProperty(std::shared_ptr<Tile> property) {
        ownedProperties.push_back(property);
        property->setOwner(std::make_shared<Player>(*this));  // Transfer ownership
    
        if (auto street = std::dynamic_pointer_cast<StreetTile>(property)) {
            adjustMoney(-street->getBasePrice());
        } else if (auto railroad = std::dynamic_pointer_cast<RailroadTile>(property)) {
            adjustMoney(-railroad->getPrice());
        } else if (property->getTileType() == "Utility") {
            adjustMoney(-150);  // Assuming utility costs 150
            incrementUtilitiesOwned();  // Increment the count of utilities owned
        }
    }

    // Offer to buy a property
    void offerToBuy(std::shared_ptr<Tile> property);

    // Pay rent to another player
    void payRent(Player& owner, int rentAmount) {
    if (money >= rentAmount) {
        // Player has enough money to pay rent
        adjustMoney(-rentAmount);  // Deduct rent from the current player
        owner.adjustMoney(rentAmount);  // Add rent to the owner
        std::cout << getName() << " paid $" << rentAmount << " in rent to " << owner.getName() << ".\n";
    } else {
        // Player doesn't have enough money, trigger bankruptcy
        std::cout << getName() << " can't afford the rent of $" << rentAmount << " and is bankrupt!\n";
        declareBankruptcy(owner);
    }
}

void declareBankruptcy(Player& owner) {
    std::cout << getName() << " is bankrupt and must transfer all properties to " << owner.getName() << ".\n";
    for (auto& property : ownedProperties) {
        property->setOwner(std::make_shared<Player>(owner));
        std::cout << owner.getName() << " now owns " << property->getName() << ".\n";
    }
    ownedProperties.clear();
    money = 0;
}




    // Pay tax
    void payTax(int taxAmount) {
        adjustMoney(-taxAmount);  // Deduct tax from player
    }

    // Handle Chance/Community Chest cards
    void receiveChanceCard(const std::string& card) {
        std::cout << "Player " << name << " received a Chance card: " << card << std::endl;
    }

    void receiveCommunityChestCard(const std::string& card) {
        std::cout << "Player " << name << " received a Community Chest card: " << card << std::endl;
    }

    // Display player info
    void displayPlayerInfo() const {
        std::cout << "Player: " << name << "\nMoney: $" << money << "\nPosition: " << position << "\n";
        std::cout << "Properties owned: \n";
        for (const auto& property : ownedProperties) {
            std::cout << "- " << property->getName() << " (" << property->getTileType() << ")\n";
        }
        std::cout << std::endl;
    }

    
};

#endif // PLAYER_HPP
