#ifndef SPECIAL_TILES_HPP
#define SPECIAL_TILES_HPP

#include "tile.hpp"
#include "player.hpp"
#include "cards.hpp"
#include <vector>
#include <cstdlib>

// Base class for special tiles
class SpecialTile : public Tile {
public:
    SpecialTile(const std::string& name)
        : Tile(name, "Special") {}

    void onLand(std::shared_ptr<Player> player, Game &game) override {
        // Default behavior, can be overridden by subclasses
    }
    
};

class UtilityTile : public Tile {
public:
    UtilityTile(const std::string& name)
        : Tile(name, "Utility") {}

    // Get the price of the utility
    int getPrice() const { return 150; }

    // Calculate rent based on the dice roll result
    int calculateRent(int diceRoll, int utilitiesOwned) const {
        if (utilitiesOwned == 1) {
            return 4 * diceRoll;  // Rent if 1 utility is owned
        } else if (utilitiesOwned == 2) {
            return 10 * diceRoll;  // Rent if 2 utilities are owned
        }
        return 0;
    }

    // Overriding the onLand method
    void onLand(std::shared_ptr<Player> player, Game& game) override {
    if (isOccupied()) {
        // Check if player is not the owner
        if (player != owner) {
            // Player pays rent based on dice roll
            int diceRoll = player->getLastDiceRoll();
            int utilitiesOwned = owner->getNumberOfUtilities();
            int rent = calculateRent(diceRoll, utilitiesOwned);
            std::cout << "Player " << player->getName() << " landed on " << name 
                      << " and pays $" << rent << " to " << owner->getName() << std::endl;
            player->payRent(*owner, rent);
        }
    } else {
        // Offer player the option to buy the utility
        std::cout << player->getName() << ", do you want to buy " << getName() << "? (Price: $" << getPrice() << ")\n";
        if (player->getMoney() >= getPrice()) {
            player->buyProperty(shared_from_this());
            setOwner(player);  // Set the owner after purchase
            std::cout << player->getName() << " has bought " << getName() << "!\n";
        } else {
            std::cout << player->getName() << " doesn't have enough money to buy " << getName() << ".\n";
        }
    }
}

};


class ChanceTile : public SpecialTile {
private:
    std::vector<std::shared_ptr<Card>> chanceCards;  // Store actual cards instead of strings

public:
    ChanceTile(const std::string& name)
        : SpecialTile(name) {
        // Initialize Chance cards
        chanceCards.push_back(std::make_shared<AdvanceToGoCard>());
        chanceCards.push_back(std::make_shared<GoToJailCard>());
        chanceCards.push_back(std::make_shared<TripToReadingRailroadCard>());
        chanceCards.push_back(std::make_shared<GeneralRepairsCard>());
        chanceCards.push_back(std::make_shared<GetOutOfJailFreeCard>());
        chanceCards.push_back(std::make_shared<AdvanceToNearestUtilityCard>());
        chanceCards.push_back(std::make_shared<AdvanceToNearestRailroadCard>());
    }

    void onLand(std::shared_ptr<Player> player, Game& game) override {
        int randomIndex = std::rand() % chanceCards.size();
        player->handleChanceCard(chanceCards[randomIndex], game);
    }
};

class CommunityChestTile : public SpecialTile {
private:
    std::vector<std::shared_ptr<Card>> communityChestCards;

public:
    CommunityChestTile(const std::string& name)
        : SpecialTile(name) {
        // Initialize Community Chest cards
        communityChestCards.push_back(std::make_shared<AdvanceToGoCard>());
        communityChestCards.push_back(std::make_shared<GeneralRepairsCard>());
        communityChestCards.push_back(std::make_shared<GetOutOfJailFreeCard>());
        communityChestCards.push_back(std::make_shared<GeneralRepairsCard>());
        communityChestCards.push_back(std::make_shared<GeneralRepairsCard>());
    }

    void onLand(std::shared_ptr<Player> player, Game& game) override {
        int randomIndex = std::rand() % communityChestCards.size();
        player->handleCommunityChestCard(communityChestCards[randomIndex], game);
    }
};



class TaxTile : public SpecialTile {
private:
    const int taxAmount = 100;

public:
    TaxTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player, Game& game) override {
        player->payTax(taxAmount);  // Assuming this method exists in Player
    }
};


class FreeParkingTile : public SpecialTile {
public:
    FreeParkingTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player, Game& game) override {
        // Nothing happens, player just rests here
    }
};


class GoToJailTile : public SpecialTile {
public:
    GoToJailTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player, Game& game) override {
        player->goToJail();  // Assuming this method exists in Player
    }
};


class StartTile : public SpecialTile {
public:
    StartTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player, Game& game) override {
        player->collectFromStart(200);  // Assuming this method exists in Player
    }
};


class JailTile : public SpecialTile {
public:
    JailTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player, Game& game) override {
        if (player->isInJail()) {
            player->handleJailTurn();  // Assuming this method exists in Player
        } else {
            // Just visiting, no action required
        }
    }
};

#endif // SPECIAL_TILES_HPP
