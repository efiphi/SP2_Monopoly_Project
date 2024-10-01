#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include <memory>

// Forward declare Player to avoid circular dependency
class Player;
class Game;

class Tile : public std::enable_shared_from_this<Tile> {
protected:
    std::string name;
    std::string tileType;
    std::shared_ptr<Player> owner;

public:
    Tile(const std::string& name, const std::string& type) : name(name), tileType(type), owner(nullptr) {}

    std::string getName() const { return name; }
    std::string getTileType() const { return tileType; }

    bool isOccupied() const { return owner != nullptr; }
    void setOwner(std::shared_ptr<Player> newOwner) { owner = newOwner; }
    std::shared_ptr<Player> getOwner() const {return owner; }

    virtual void onLand(std::shared_ptr<Player> player, Game& game) = 0; // Pure virtual function

    virtual ~Tile() = default; //Destructor

    
};

#endif // TILE_HPP
