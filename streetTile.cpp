#include "streetTile.hpp"
#include "player.hpp"
#include <iostream>

void StreetTile::onLand(Player& player) {
    if (owner == nullptr) {
        std::cout << player.getName() << ", do you want to buy " << getName() << "? (Price: $" << basePrice << ")\n";
        if (player.getMoney() >= basePrice) {
            player.buyProperty(shared_from_this());  // Use shared_from_this from Tile
            setOwner(player.shared_from_this());  // Set the player as the owner
            std::cout << player.getName() << " has bought " << getName() << "!\n";
        } else {
            std::cout << player.getName() << " doesn't have enough money to buy " << getName() << ".\n";
        }
    } else if (owner->getName() != player.getName()) {
        int rent = calculateRent();
        std::cout << player.getName() << " landed on " << owner->getName() << "'s property and must pay $" << rent << " in rent.\n";
        player.payRent(*owner, rent);
    }
}


