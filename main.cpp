#include <iostream>
#include "game.hpp"
#include "streetTile.hpp"
#include "railroadTile.hpp"
#include "specialTiles.hpp"
#include "dice.hpp"
#include "board.hpp"
#include "player.hpp"

int main() {
    // Create players
    std::shared_ptr<Player> player1 = std::make_shared<Player>("Alice", 1500);
    std::shared_ptr<Player> player2 = std::make_shared<Player>("Bob", 1500);

    // Create a vector of players (shared_ptr)
    std::vector<std::shared_ptr<Player>> playerList = {player1, player2};

    // Create a game instance with the player list
    Game monopolyGame(playerList);

    // Add tiles to the board

    // Bottom Row (Right to Left)
    monopolyGame.addTile(std::make_shared<StartTile>("Start"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Sydney", "Brown", 60, 2));
    monopolyGame.addTile(std::make_shared<CommunityChestTile>("Community Chest 1"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Melbourne", "Brown", 60, 4));
    monopolyGame.addTile(std::make_shared<TaxTile>("Income Tax"));
    monopolyGame.addTile(std::make_shared<RailroadTile>("Reading Railroad"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Toronto", "Light Blue", 100, 6));
    monopolyGame.addTile(std::make_shared<ChanceTile>("Chance 1"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Vancouver", "Light Blue", 100, 6));
    monopolyGame.addTile(std::make_shared<StreetTile>("Montreal", "Light Blue", 120, 8));

    // Left Side (Bottom to Top)
    monopolyGame.addTile(std::make_shared<JailTile>("Jail"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Rio de Janeiro", "Pink", 140, 10));
    monopolyGame.addTile(std::make_shared<UtilityTile>("Electric Company"));
    monopolyGame.addTile(std::make_shared<StreetTile>("São Paulo", "Pink", 140, 10));
    monopolyGame.addTile(std::make_shared<StreetTile>("Brasilia", "Pink", 160, 12));
    monopolyGame.addTile(std::make_shared<RailroadTile>("Pennsylvania Railroad"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Berlin", "Orange", 180, 14));
    monopolyGame.addTile(std::make_shared<CommunityChestTile>("Community Chest 2"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Munich", "Orange", 180, 14));
    monopolyGame.addTile(std::make_shared<StreetTile>("Hamburg", "Orange", 200, 16));

    // Top Row (Left to Right)
    monopolyGame.addTile(std::make_shared<FreeParkingTile>("Free Parking"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Paris", "Red", 220, 18));
    monopolyGame.addTile(std::make_shared<ChanceTile>("Chance 2"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Lyon", "Red", 220, 18));
    monopolyGame.addTile(std::make_shared<StreetTile>("Marseille", "Red", 240, 20));
    monopolyGame.addTile(std::make_shared<RailroadTile>("B&O Railroad"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Barcelona", "Yellow", 260, 22));
    monopolyGame.addTile(std::make_shared<StreetTile>("Madrid", "Yellow", 260, 22));
    monopolyGame.addTile(std::make_shared<UtilityTile>("Water Works"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Malaga", "Yellow", 280, 24));

    // Right Side (Top to Bottom)
    monopolyGame.addTile(std::make_shared<GoToJailTile>("Go to Jail"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Milano", "Green", 300, 26));
    monopolyGame.addTile(std::make_shared<StreetTile>("Rome", "Green", 300, 26));
    monopolyGame.addTile(std::make_shared<CommunityChestTile>("Community Chest 3"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Venice", "Green", 320, 28));
    monopolyGame.addTile(std::make_shared<RailroadTile>("Short Line"));
    monopolyGame.addTile(std::make_shared<ChanceTile>("Chance 3"));
    monopolyGame.addTile(std::make_shared<StreetTile>("Los Angeles", "Blue", 350, 35));
    monopolyGame.addTile(std::make_shared<TaxTile>("Luxury Tax"));
    monopolyGame.addTile(std::make_shared<StreetTile>("New York", "Blue", 400, 50));

    // Display a welcome message
    std::cout << "Welcome to Monopoly with Cities from Around the World!\n";

    // Start the game
    monopolyGame.start();

    return 0;
}
