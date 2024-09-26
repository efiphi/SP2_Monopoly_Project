#include "railroadTile.hpp"
#include "player.hpp"
#include <iostream>

void RailroadTile::onLand(std::shared_ptr<Player> player) {
    if (owner == nullptr) {
        std::cout << player->getName() << ", do you want to buy " << getName() << "? (Price: $" << price << ")\n";
        if (player->getMoney() >= price) {
            player->buyProperty(shared_from_this());  // Use shared_from_this from Tile
            setOwner(player->shared_from_this());  // Set player as owner
            std::cout << player->getName() << " has bought " << getName() << "!\n";
        } else {
            std::cout << player->getName() << " doesn't have enough money to buy " << getName() << ".\n";
        }
    } else if (owner != nullptr) {
        int rent = calculateRent();
        std::cout << player->getName() << " landed on " << owner->getName() << "'s railroad and must pay $" << rent << " in rent.\n";
        player->payRent(*owner, rent);
    }
}


