#include "player.hpp"
#include "tile.hpp"
#include "cards.hpp"
#include "game.hpp"
#include <iostream>
#include <memory>

void Player::offerToBuy(std::shared_ptr<Tile> property) {
    // Check if the property is a StreetTile
    if (auto street = std::dynamic_pointer_cast<StreetTile>(property)) {
        std::cout << "Player " << name << ", do you want to buy " << street->getName() 
                  << "? (Price: $" << street->getBasePrice() << ")\n";
        // Simulate a decision and proceed with the purchase
        if (money >= street->getBasePrice()) {
            buyProperty(property);
            std::cout << name << " has bought " << street->getName() << "!\n";
        } else {
            std::cout << name << " doesn't have enough money to buy " << street->getName() << ".\n";
        }
    }
    // Check if the property is a RailroadTile
    else if (auto railroad = std::dynamic_pointer_cast<RailroadTile>(property)) {
        std::cout << "Player " << name << ", do you want to buy " << railroad->getName() 
                  << "? (Price: $" << railroad->getPrice() << ")\n";
        // Simulate a decision and proceed with the purchase
        if (money >= railroad->getPrice()) {
            buyProperty(property);
            std::cout << name << " has bought " << railroad->getName() << "!\n";
        } else {
            std::cout << name << " doesn't have enough money to buy " << railroad->getName() << ".\n";
        }
    }
    // Check if the property is a UtilityTile
    else if (property->getTileType() == "Utility") {
        std::cout << "Player " << name << ", do you want to buy " << property->getName() 
                  << "? (Price: $150)\n";
        // Simulate a decision and proceed with the purchase
        if (money >= 150) {
            buyProperty(property);
            std::cout << name << " has bought " << property->getName() << "!\n";
        } else {
            std::cout << name << " doesn't have enough money to buy " << property->getName() << ".\n";
        }
    }
}


void Player::handleChanceCard(std::shared_ptr<Card> card, Game& game) {
    std::cout << name << " has drawn a Chance card: " << card->getDescription() << "\n";
    card->execute(shared_from_this(), game);  // Execute the effect of the Chance card
}

void Player::handleCommunityChestCard(std::shared_ptr<Card> card, Game& game) {
    std::cout << name << " has drawn a Community Chest card: " << card->getDescription() << "\n";
    card->execute(shared_from_this(), game);  // Execute the effect of the Community Chest card
}
