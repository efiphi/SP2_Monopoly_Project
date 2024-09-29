#ifndef SPECIAL_TILES_HPP
#define SPECIAL_TILES_HPP

#include "tile.hpp"
#include "player.hpp"
#include <vector>
#include <cstdlib>

// Base class for special tiles
class SpecialTile : public Tile {
public:
    SpecialTile(const std::string& name)
        : Tile(name, "Special") {}

    void onLand(std::shared_ptr<Player> player) override {
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
    void onLand(std::shared_ptr<Player> player) override {
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
    std::vector<std::string> chanceCards;

public:
    ChanceTile(const std::string& name)
        : SpecialTile(name) {
        chanceCards = {
            "Get out of jail free card",
            "Advance to Go (Collect $200)",
            "Pay a $50 fine",
            "Go back 3 spaces",
            "Go directly to jail (do not pass Go, do not collect $200)",
            "Collect $150 from all players"
        };
    }

    void onLand(std::shared_ptr<Player> player) override {
        int randomIndex = std::rand() % chanceCards.size();
        std::string drawnCard = chanceCards[randomIndex];
        player->receiveChanceCard(drawnCard);  // Assuming this method exists in Player
    }
};

class CommunityChestTile : public SpecialTile {
private:
    std::vector<std::string> communityChestCards;

public:
    CommunityChestTile(const std::string& name)
        : SpecialTile(name) {
        communityChestCards = {
            "Collect $200",
            "Pay hospital fees of $100",
            "Go directly to jail",
            "Receive $50 as a birthday gift",
            "Pay school fees of $150",
            "Collect $10 from each player"
        };
    }

    void onLand(std::shared_ptr<Player> player) override {
        int randomIndex = std::rand() % communityChestCards.size();
        std::string drawnCard = communityChestCards[randomIndex];
        player->receiveCommunityChestCard(drawnCard);  // Placeholder for card logic
    }
};



class TaxTile : public SpecialTile {
private:
    const int taxAmount = 100;

public:
    TaxTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player) override {
        player->payTax(taxAmount);  // Assuming this method exists in Player
    }
};


class FreeParkingTile : public SpecialTile {
public:
    FreeParkingTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player) override {
        // Nothing happens, player just rests here
    }
};


class GoToJailTile : public SpecialTile {
public:
    GoToJailTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player) override {
        player->goToJail();  // Assuming this method exists in Player
    }
};


class StartTile : public SpecialTile {
public:
    StartTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player) override {
        player->collectFromStart(200);  // Assuming this method exists in Player
    }
};


class JailTile : public SpecialTile {
public:
    JailTile(const std::string& name)
        : SpecialTile(name) {}

    void onLand(std::shared_ptr<Player> player) override {
        if (player->isInJail()) {
            player->handleJailTurn();  // Assuming this method exists in Player
        } else {
            // Just visiting, no action required
        }
    }
};

#endif // SPECIAL_TILES_HPP
