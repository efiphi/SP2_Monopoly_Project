#include "railroadTile.hpp"
#include "player.hpp"
#include <iostream>

void RailroadTile::onLand(std::shared_ptr<Player> player, Game& game) {
    if (owner == nullptr) {
        // Player can buy the railroad
        std::cout << player->getName() << ", do you want to buy " << getName() << "? (Price: $" << price << ")\n";
        if (player->getMoney() >= price) {
            player->buyProperty(shared_from_this());  // Player buys the railroad
            setOwner(player);  // Set the current player as the owner
            std::cout << player->getName() << " has bought " << getName() << "!\n";
        } else {
            std::cout << player->getName() << " doesn't have enough money to buy " << getName() << ".\n";
        }
    } else if (owner == player) {
        // Player landed on their own railroad
        std::cout << player->getName() << " landed on their own railroad and does not pay rent.\n";
    } else {
        // Player landed on another player's railroad, pay rent
        int rent = calculateRent();
        std::cout << player->getName() << " landed on " << owner->getName() << "'s railroad and must pay $" << rent << " in rent.\n";
        player->payRent(*owner, rent);  // Player pays rent to the owner
    }
}



