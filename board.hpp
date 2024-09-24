#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <memory>
#include <iostream>
#include "tile.hpp"

class Board {
private:
    std::vector<std::shared_ptr<Tile>> tiles;

public:
    // Add a tile to the board
    void addTile(std::shared_ptr<Tile> tile) {
        if (tiles.size() < 40) {
            tiles.push_back(tile);
        }
    }

    // Remove a tile by its index
    bool removeTile(int index) {
        if (index >= 0 && index < static_cast<int>(tiles.size())) {
            tiles.erase(tiles.begin() + index);
            return true;
        }
        return false;
    }

    int getTileCount() const {
        return tiles.size();
    }

    // Get a tile by its position
    std::shared_ptr<Tile> getTile(int position) const {
        if (position >= 0 && position < static_cast<int>(tiles.size())) {
            return tiles[position];
        }
        return nullptr;
    }

    // Display the entire board (for debug purposes)
    void displayBoard() const {
        for (int i = 0; i < static_cast<int>(tiles.size()); ++i) {
            std::cout << i << ": " << tiles[i]->getName() << " (" << tiles[i]->getTileType() << ")" << std::endl;
        }
    }
};

#endif // BOARD_HPP
