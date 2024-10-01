#include "streetTile.hpp"
#include "player.hpp"
#include <iostream>

void StreetTile::onLand(std::shared_ptr<Player> player, Game& game) {
    if (owner == nullptr) {
        // Player can buy the property
        std::cout << player->getName() << ", do you want to buy " << getName() << "? (Price: $" << basePrice << ")\n";
        if (player->getMoney() >= basePrice) {
            player->buyProperty(shared_from_this());  // Player buys the property
            setOwner(player);  // Set the current player as the owner
            std::cout << player->getName() << " has bought " << getName() << "!\n";
        } else {
            std::cout << player->getName() << " doesn't have enough money to buy " << getName() << ".\n";
        }
    } else if (owner == player) {
        // Player landed on their own property
        std::cout << player->getName() << " landed on their own property.\n";
    } else {
        // Player landed on another player's property, pay rent
        int rent = calculateRent();
        std::cout << player->getName() << " landed on " << owner->getName() << "'s property and must pay $" << rent << " in rent.\n";
        player->payRent(*owner, rent);  // Player pays rent to the owner
    }
}



