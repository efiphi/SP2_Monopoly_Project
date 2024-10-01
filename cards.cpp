#include "cards.hpp"
#include "player.hpp"
#include "game.hpp"
#include <iostream>


// 1. Advance to Go (Collect $200)
std::string AdvanceToGoCard::getDescription() const {
    return "Advance to Go (Collect $200)";
}

void AdvanceToGoCard::execute(std::shared_ptr<Player> player, Game& game) {
    player->setPosition(0);  // Move to Start position
    player->adjustMoney(200);  // Collect $200
    std::cout << player->getName() << " advances to Go and collects $200.\n";
}

// 2. Go to Jail (Do not pass Go, do not collect $200)
std::string GoToJailCard::getDescription() const {
    return "Go directly to Jail – do not pass Go, do not collect $200";
}

void GoToJailCard::execute(std::shared_ptr<Player> player, Game& game) {
    player->goToJail();  // Move player to jail position
    std::cout << player->getName() << " goes directly to Jail. Do not pass Go, do not collect $200.\n";
}

// 3. Get Out of Jail Free
std::string GetOutOfJailFreeCard::getDescription() const {
    return "Get out of Jail Free – This card may be kept until needed or traded";
}

void GetOutOfJailFreeCard::execute(std::shared_ptr<Player> player, Game& game) {
    player->receiveGetOutOfJailCard();  // Grant player a "Get out of Jail Free" card
    std::cout << player->getName() << " receives a Get Out of Jail Free card.\n";
}

// 4. Take a Trip to Reading Railroad
std::string TripToReadingRailroadCard::getDescription() const {
    return "Take a trip to Reading Railroad – If you pass Go, collect $200";
}

void TripToReadingRailroadCard::execute(std::shared_ptr<Player> player, Game& game) {
    int startPosition = player->getPosition();
    int targetPosition = 5;  // Reading Railroad position

    if (startPosition > targetPosition) {
        player->adjustMoney(200);  // Collect $200 for passing Go
        std::cout << player->getName() << " passes Go and collects $200.\n";
    }

    player->setPosition(targetPosition);  // Move to Reading Railroad
    std::cout << player->getName() << " takes a trip to Reading Railroad.\n";

    // Fetch the tile at the target position and check ownership
    auto tile = std::dynamic_pointer_cast<RailroadTile>(game.getTile(targetPosition));
    if (tile) {
        if (tile->getOwner() == nullptr) {
            // Property is not owned: offer player to buy
            if (player->getMoney() >= 200) {
                std::cout << player->getName() << ", do you want to buy " << tile->getName()
                          << "? (Price: $" << 200 << ")\n";
                player->buyProperty(tile);
            } else {
                std::cout << player->getName() << " does not have enough money to buy " << tile->getName() << ".\n";
            }
        } else if (tile->getOwner() != player) {
            // Property is owned by another player: pay rent
            int rent = tile->calculateRent();
            std::cout << player->getName() << " landed on " << tile->getOwner()->getName() << "'s railroad and must pay $" << rent << " in rent.\n";
            auto owner = tile->getOwner();  // Store the owner in a separate variable
            player->payRent(*owner, rent);  


        }
    }
}


// 5. Advance to Nearest Utility
std::string AdvanceToNearestUtilityCard::getDescription() const {
    return "Advance to nearest Utility – If unowned, you may buy it. If owned, pay owner 10 times the dice roll.";
}

void AdvanceToNearestUtilityCard::execute(std::shared_ptr<Player> player, Game& game) {
    Board& board = game.getBoard();
    int currentPos = player->getPosition();
    int nearestUtilityPos = -1;
    int tileCount = board.getTileCount();

    // Find the nearest Utility
    for (int i = (currentPos + 1) % tileCount; i != currentPos; i = (i + 1) % tileCount) {
        auto utilityTile = std::dynamic_pointer_cast<UtilityTile>(board.getTile(i));
        if (utilityTile) {
            nearestUtilityPos = i;
            break;
        }
    }

    if (nearestUtilityPos == -1) return;  // No utility found, return

    player->setPosition(nearestUtilityPos);  // Move the player to the nearest utility
    auto utilityTile = std::dynamic_pointer_cast<UtilityTile>(board.getTile(nearestUtilityPos));

    std::cout << player->getName() << " advances to " << utilityTile->getName() << " at position " << nearestUtilityPos << "\n";
    std::cout << "Current owner: " << (utilityTile->isOccupied() ? utilityTile->getOwner()->getName() : "None") << "\n";

    if (!utilityTile->isOccupied()) {
        // Offer to buy if unowned
        player->buyProperty(utilityTile);
        std::cout << player->getName() << " has successfully bought " << utilityTile->getName() << ".\n";
    } else {
        // Pay rent if owned
        std::shared_ptr<Player> owner = utilityTile->getOwner();
        int diceRoll = player->getLastDiceRoll();  // Use the last dice roll value for rent calculation
        int rent = utilityTile->calculateRent(diceRoll, owner->getNumberOfUtilities());
        std::cout << player->getName() << " pays $" << rent << " in rent to " << owner->getName() << ".\n";
        player->payRent(*owner, rent);
    }
}



// 6. Advance to Nearest Railroad
std::string AdvanceToNearestRailroadCard::getDescription() const {
    return "Advance to nearest Railroad – If unowned, you may buy it. If owned, pay owner twice the rent.";
}

void AdvanceToNearestRailroadCard::execute(std::shared_ptr<Player> player, Game& game) {
    Board& board = game.getBoard();
    int currentPos = player->getPosition();
    int nearestRailroadPos = -1;
    int tileCount = board.getTileCount();

    // Find the nearest Railroad
    for (int i = (currentPos + 1) % tileCount; i != currentPos; i = (i + 1) % tileCount) {
        auto railroadTile = std::dynamic_pointer_cast<RailroadTile>(board.getTile(i));
        if (railroadTile) {
            nearestRailroadPos = i;
            break;
        }
    }

    if (nearestRailroadPos == -1) return;  // No railroad found, return

    player->setPosition(nearestRailroadPos);
    auto railroadTile = std::dynamic_pointer_cast<RailroadTile>(board.getTile(nearestRailroadPos));

    std::cout << player->getName() << " advances to " << railroadTile->getName() << "\n";

    if (!railroadTile->isOccupied()) {
        // Offer to buy if unowned
        player->buyProperty(railroadTile);
    } else {
        // Pay rent if owned
        int rent = 100;  // Pay double the base rent
        std::cout << player->getName() << " pays $" << rent << " in rent to " << railroadTile->getOwner()->getName() << "\n";
        player->payRent(*railroadTile->getOwner(), rent);
    }
}

// 7. General Repairs Card
std::string GeneralRepairsCard::getDescription() const {
    return "Make general repairs on all your property – For each house pay $25 – For each hotel $100";
}

void GeneralRepairsCard::execute(std::shared_ptr<Player> player, Game& game) {
    int houseCount = player->getHouseCount();
    int hotelCount = player->getHotelCount();
    int totalRepairCost = (houseCount * 25) + (hotelCount * 100);

    player->adjustMoney(-totalRepairCost);
    std::cout << player->getName() << " pays $" << totalRepairCost << " for general repairs ($25/house, $100/hotel).\n";
}
