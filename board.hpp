#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "tile.hpp"
#include "streetTile.hpp"


class Board {
private:
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<sf::Vector2f> tilePositions;  // Stores graphical positions for each tile

    // Private constructor (Singleton pattern)
    Board();

public:
    // Singleton access method
    static Board& getInstance() {
        static Board instance;
        return instance;
    }


    // Delete copy constructor and assignment to prevent duplication
    Board(const Board&) = delete;
    void operator=(const Board&) = delete;

    // Add a tile to the board with its graphical position
    void addTile(std::shared_ptr<Tile> tile, const sf::Vector2f& position) {
        if (tiles.size() < 40) {
            tiles.push_back(tile);
            tilePositions.push_back(position);  // Add the graphical position
        }
    }

    // Remove a tile by its index
    bool removeTile(int index) {
        if (index >= 0 && index < static_cast<int>(tiles.size())) {
            tiles.erase(tiles.begin() + index);
            tilePositions.erase(tilePositions.begin() + index);  // Remove the graphical position
            return true;
        }
        return false;
    }


    // Get the number of tiles
    int getTileCount() const { return static_cast<int>(tiles.size()); }

    std::vector<StreetTile*> getColorGroupProperties(const std::string& colorGroup) const;

    std::shared_ptr<Tile> findPropertyByName(const std::string& propertyName) const {
        for (const auto& tile : tiles) {
            if (tile->getName() == propertyName) {
                return tile;
            }
        }
        return nullptr;
    }



    // Get a tile by its position
    std::shared_ptr<Tile> getTile(int position) const {
        if (position >= 0 && position < static_cast<int>(tiles.size())) {
            return tiles[position];
        }
        return nullptr;
    }

    // Get the graphical position of a tile
    sf::Vector2f getTilePosition(int index) const {
        if (index >= 0 && index < static_cast<int>(tilePositions.size())) {
            return tilePositions[index];
        }
        return sf::Vector2f(0, 0);  // Return (0,0) if the index is invalid
    }

    // Display the entire board (for debug purposes)
    void displayBoard() const {
        for (int i = 0; i < static_cast<int>(tiles.size()); ++i) {
            std::cout << i << ": " << tiles[i]->getName() << " (" << tiles[i]->getTileType() << ")" << std::endl;
        }
    }
};

#endif // BOARD_HPP
