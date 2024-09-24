#ifndef STREET_TILE_HPP
#define STREET_TILE_HPP

#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <memory>
#include "tile.hpp"

// Forward declare Player to avoid circular dependency
class Player;

class StreetTile : public Tile{
private:
    std::string colorGroup;  // The color group this street belongs to
    int basePrice;           // Base price of the street
    int baseRent;            // Base rent for the street without any houses or hotels
    int houses;              // Number of houses built (0 to 4)
    bool hasHotel;           // Whether the street has a hotel

public:
    // Constructor
    StreetTile(const std::string& name, const std::string& colorGroup, int basePrice, int baseRent)
        : Tile(name, "Street"), colorGroup(colorGroup), basePrice(basePrice), baseRent(baseRent), houses(0), hasHotel(false) {}

    // Getters for price and rent
    int getBasePrice() const { return basePrice; }
    int getBaseRent() const { return baseRent; }
    std::string getColorGroup() const { return colorGroup; }

    // Get the current number of houses
    int getHouseCount() const { return houses; }

    // Check if a hotel is built
    bool isHotelBuilt() const { return hasHotel; }

    // Calculate the current rent dynamically based on houses or hotel
    int calculateRent() const {
        if (hasHotel) {
            return baseRent * std::pow(2, 5); // Rent with hotel: baseRent * 2^4
        } else if (houses > 0) {
            return baseRent * std::pow(2, houses); // Rent with houses: baseRent * 2^(houses-1)
        } else {
            return baseRent; // Rent with no houses or hotel
        }
    }

    // Method to build a house (adds 1 house if possible and all color group streets have the same or fewer houses)
    bool buildHouse(const std::vector<StreetTile*>& colorGroupTiles) {
        int minHouses = (*std::min_element(colorGroupTiles.begin(), colorGroupTiles.end(),
                            [](StreetTile* a, StreetTile* b) {
                                return a->getHouseCount() < b->getHouseCount();
                            }))->getHouseCount();

        // If this street already has more houses than the others, it cannot build another house
        if (houses > minHouses) {
            return false; // Cannot build a house, as other streets in the group don't have enough houses
        }

        if (houses < 4 && !hasHotel) {
            houses++;
            return true;
        }
        return false;
    }

    // Method to build a hotel (only if all streets in the color group have 4 houses)
    bool buildHotel(const std::vector<StreetTile*>& colorGroupTiles) {
        bool allHaveMaxHouses = std::all_of(colorGroupTiles.begin(), colorGroupTiles.end(), [](StreetTile* tile) {
            return tile->getHouseCount() == 4;
        });

        if (!allHaveMaxHouses || hasHotel) {
            return false;
        }

        hasHotel = true;
        houses = 0; // Reset houses since the hotel takes over
        return true;
    }

    // Calculate the cost of a house
    int houseCost() const {
        return basePrice; // The cost of a house is the same as the base price of the street
    }

    // Calculate the cost of a hotel
    int hotelCost() const {
        return (basePrice * 4) + 100; // Hotel cost: base price * 4 + 100
    }

    // Define what happens when a player lands on this street
    void onLand(Player& player) override;
};

#endif // STREET_TILE_HPP
