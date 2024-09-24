#ifndef RAILROAD_TILE_HPP
#define RAILROAD_TILE_HPP

#include "tile.hpp"

// Forward declaration of Player to avoid circular dependency
class Player;

class RailroadTile : public Tile{
private:
    const int price = 200;   // The fixed purchase price of a railroad
    const int baseRent = 50; // Base rent for owning one railroad
    int numberOfRailroadsOwned; // Tracks how many railroads the owner has

public:
    // Constructor
    RailroadTile(const std::string& name)
        : Tile(name, "Railroad"), numberOfRailroadsOwned(1) {}

    // Get the fixed price of the railroad
    int getPrice() const {
        return price;
    }

    // Calculate rent based on how many railroads the owner has
    int calculateRent() const {
        if (numberOfRailroadsOwned == 0) {
            return baseRent;  // Return base rent if no railroads are set (prevent division issues)
        }
        return baseRent * numberOfRailroadsOwned;  // Rent is 50 * the number of railroads owned
    }

    // Update the number of railroads the owner has
    void setNumberOfRailroadsOwned(int number) {
        numberOfRailroadsOwned = number;
    }

    // Define what happens when a player lands on this railroad
    void onLand(Player& player) override;
};

#endif // RAILROAD_TILE_HPP
