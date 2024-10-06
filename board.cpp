#include "board.hpp"
#include "streetTile.hpp"
#include "railroadTile.hpp"
#include "specialTiles.hpp"


Board::Board() {
    // Initialize all 40 tiles on the board
    addTile(std::make_shared<StartTile>("Go"), {750, 741});                      // 0
    addTile(std::make_shared<StreetTile>("Mediterranean Ave", "Brown", 60, 2), {655, 741});  // 1
    addTile(std::make_shared<CommunityChestTile>("Community Chest"), {600, 741}); // 2
    addTile(std::make_shared<StreetTile>("Baltic Ave", "Brown", 60, 4), {540, 741});    // 3
    addTile(std::make_shared<TaxTile>("Income Tax"), {475, 741});                // 4
    addTile(std::make_shared<RailroadTile>("Reading Railroad"), {405, 741});     // 5
    addTile(std::make_shared<StreetTile>("Oriental Ave", "Light Blue", 100, 6), {330, 741}); // 6
    addTile(std::make_shared<ChanceTile>("Chance"), {250, 741});                 // 7
    addTile(std::make_shared<StreetTile>("Vermont Ave", "Light Blue", 100, 6), {200, 741}); // 8
    addTile(std::make_shared<StreetTile>("Connecticut Ave", "Light Blue", 120, 8), {140, 741}); // 9

    // Left column (bottom to top)
    addTile(std::make_shared<JailTile>("Jail"), {50, 741});                      // 10
    addTile(std::make_shared<StreetTile>("St. Charles Place", "Pink", 140, 10), {50, 655}); // 11
    addTile(std::make_shared<UtilityTile>("Electric Company"), {50, 600});       // 12
    addTile(std::make_shared<StreetTile>("States Ave", "Pink", 140, 10), {50, 540}); // 13
    addTile(std::make_shared<StreetTile>("Virginia Ave", "Pink", 160, 12), {50, 475}); // 14
    addTile(std::make_shared<RailroadTile>("Pennsylvania Railroad"), {50, 405}); // 15
    addTile(std::make_shared<StreetTile>("St. James Place", "Orange", 180, 14), {50, 330}); // 16
    addTile(std::make_shared<CommunityChestTile>("Community Chest"), {50, 250}); // 17
    addTile(std::make_shared<StreetTile>("Tennessee Ave", "Orange", 180, 14), {50, 200}); // 18
    addTile(std::make_shared<StreetTile>("New York Ave", "Orange", 200, 16), {50, 140}); // 19

    // Top row (left to right)
    addTile(std::make_shared<FreeParkingTile>("Free Parking"), {50, 50});        // 20
    addTile(std::make_shared<StreetTile>("Kentucky Ave", "Red", 220, 18), {140, 50}); // 21
    addTile(std::make_shared<ChanceTile>("Chance"), {200, 50});                  // 22
    addTile(std::make_shared<StreetTile>("Indiana Ave", "Red", 220, 18), {250, 50}); // 23
    addTile(std::make_shared<StreetTile>("Illinois Ave", "Red", 240, 20), {330, 50}); // 24
    addTile(std::make_shared<RailroadTile>("B&O Railroad"), {405, 50});          // 25
    addTile(std::make_shared<StreetTile>("Atlantic Ave", "Yellow", 260, 22), {475, 50}); // 26
    addTile(std::make_shared<StreetTile>("Ventnor Ave", "Yellow", 260, 22), {540, 50}); // 27
    addTile(std::make_shared<UtilityTile>("Water Works"), {600, 50});            // 28
    addTile(std::make_shared<StreetTile>("Marvin Gardens", "Yellow", 280, 24), {655, 50}); // 29

    // Right column (top to bottom)
    addTile(std::make_shared<GoToJailTile>("Go to Jail"), {750, 50});            // 30
    addTile(std::make_shared<StreetTile>("Pacific Ave", "Green", 300, 26), {750, 140}); // 31
    addTile(std::make_shared<StreetTile>("North Carolina Ave", "Green", 300, 26), {750, 200}); // 32
    addTile(std::make_shared<CommunityChestTile>("Community Chest"), {750, 250}); // 33
    addTile(std::make_shared<StreetTile>("Pennsylvania Ave", "Green", 320, 28), {750, 330}); // 34
    addTile(std::make_shared<RailroadTile>("Short Line"), {750, 405});           // 35
    addTile(std::make_shared<ChanceTile>("Chance"), {750, 475});                 // 36
    addTile(std::make_shared<StreetTile>("Park Place", "Blue", 350, 35), {750, 540}); // 37
    addTile(std::make_shared<TaxTile>("Luxury Tax"), {750, 600});                // 38
    addTile(std::make_shared<StreetTile>("Boardwalk", "Blue", 400, 50), {750, 655}); // 39
}

std::vector<StreetTile*> Board::getColorGroupProperties(const std::string& colorGroup) const {
    std::vector<StreetTile*> groupProperties;
    for (const auto& tile : tiles) {
        auto streetTile = std::dynamic_pointer_cast<StreetTile>(tile);
        if (streetTile && streetTile->getColorGroup() == colorGroup) {
            groupProperties.push_back(streetTile.get());
        }
    }
    return groupProperties;
}
