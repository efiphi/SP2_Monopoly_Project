#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "player.hpp"
#include "streetTile.hpp"
#include "railroadTile.hpp"
#include "specialTiles.hpp"
#include "board.hpp"
#include "game.hpp"
#include "dice.hpp"


// Test cases for Player class
TEST_CASE("Player class tests") {
    Player player("Alice", 1500);  // Initial setup

    SUBCASE("Initial state of player") {
        CHECK(player.getMoney() == 1500);
        CHECK(player.getName() == "Alice");
        CHECK(player.isBankrupt() == false);
        CHECK(player.getPosition() == 0);
        CHECK(player.isInJail() == false);
    }

    SUBCASE("Player money management") {
        player.adjustMoney(200);
        CHECK(player.getMoney() == 1700);

        player.adjustMoney(-300);
        CHECK(player.getMoney() == 1400);
    }

    SUBCASE("Player movement") {
        player.move(5, 40);  // Moving 5 steps
        CHECK(player.getPosition() == 5);

        player.move(36, 40); // Wrapping around the board
        CHECK(player.getPosition() == 1);  // 5 + 36 = 41 % 40 = 1
    }

    SUBCASE("Player jail status") {
        player.goToJail();
        CHECK(player.isInJail() == true);
        CHECK(player.getPosition() == 10);

        player.releaseFromJail();
        CHECK(player.isInJail() == false);
    }

    SUBCASE("Player buying property") {
        auto street = std::make_shared<StreetTile>("Park Place", "Blue", 350, 50);
        player.buyProperty(street);

        CHECK(street->isOccupied() == true);
        CHECK(street->getOwner()->getName() == "Alice");
        CHECK(player.getMoney() == 1150);  // Subtracting the price of the property
    }

    SUBCASE("Player bankruptcy") {
        player.adjustMoney(-1500);
        CHECK(player.isBankrupt() == true);
    }
}

// Test cases for StreetTile class
TEST_CASE("StreetTile class tests") {
    auto street = std::make_shared<StreetTile>("Boardwalk", "Blue", 400, 50);

    SUBCASE("Initial state of StreetTile") {
        CHECK(street->getName() == "Boardwalk");
        CHECK(street->getBasePrice() == 400);
        CHECK(street->getBaseRent() == 50);
        CHECK(street->getHouseCount() == 0);
        CHECK(street->isHotelBuilt() == false);
    }

    SUBCASE("Rent calculation without houses or hotels") {
        CHECK(street->calculateRent() == 50);
    }

    SUBCASE("Building houses and calculating rent") {
        street->buildHouse({street.get()});
        CHECK(street->getHouseCount() == 1);
        CHECK(street->calculateRent() == 100);  // Rent = baseRent * 2^(houses-1)

        street->buildHouse({street.get()});
        CHECK(street->getHouseCount() == 2);
        CHECK(street->calculateRent() == 200);
    }

    SUBCASE("Building a hotel and calculating rent") {
        // Build 4 houses first
        for (int i = 0; i < 4; ++i) {
            street->buildHouse({street.get()});
        }
        CHECK(street->getHouseCount() == 4);

        // Now build the hotel
        street->buildHotel({street.get()});
        CHECK(street->isHotelBuilt() == true);
        CHECK(street->calculateRent() == 1600);  // Rent = baseRent * 2^4
    }
}

// Test cases for Board class
TEST_CASE("Board class tests") {
    Board board;

    SUBCASE("Adding tiles to the board") {
        auto tile = std::make_shared<StreetTile>("Park Place", "Blue", 350, 50);
        board.addTile(tile);

        CHECK(board.getTileCount() == 1);
        CHECK(board.getTile(0)->getName() == "Park Place");
    }

    SUBCASE("Removing tiles from the board") {
        auto tile1 = std::make_shared<StreetTile>("Park Place", "Blue", 350, 50);
        auto tile2 = std::make_shared<StreetTile>("Boardwalk", "Blue", 400, 50);
        board.addTile(tile1);
        board.addTile(tile2);

        CHECK(board.getTileCount() == 2);
        board.removeTile(1);
        CHECK(board.getTileCount() == 1);
        CHECK(board.getTile(0)->getName() == "Park Place");
    }

    SUBCASE("Fetching non-existent tiles") {
        auto tile = board.getTile(5);  // Out of bounds
        CHECK(tile == nullptr);
    }
}

// Test cases for Game class
TEST_CASE("Game class tests") {
    auto player1 = std::make_shared<Player>("Alice", 1500);
    auto player2 = std::make_shared<Player>("Bob", 1500);
    Game game({player1, player2});

    game.addTile(std::make_shared<StreetTile>("Sydney", "Brown", 60, 2));
    game.addTile(std::make_shared<StreetTile>("Melbourne", "Brown", 60, 4));

    SUBCASE("Initial game state") {
        CHECK(game.checkForWinner() == false);
    }

    SUBCASE("Taking turns and switching players") {
        game.setDice(std::make_shared<MockDice>(1, 2));  // Mocked dice roll of 1 and 2
        game.playTurn();
        CHECK(game.getCurrentPlayer() == player2);

        game.setDice(std::make_shared<MockDice>(3, 3));  // Mocked doubles
        game.playTurn();
        CHECK(game.getCurrentPlayer() == player1);  // Player gets another turn

        game.setDice(std::make_shared<MockDice>(2, 3));  // Normal roll
        game.playTurn();
        CHECK(game.getCurrentPlayer() == player2);  // Switches to Player 2
    }

    SUBCASE("Board and player movement") {
        CHECK(game.getTileCount() > 0);

        game.setDice(std::make_shared<MockDice>(1, 2));  // Mocked dice roll to move Player 1
        game.playTurn();
        CHECK(player1->getPosition() == 3);

        game.setDice(std::make_shared<MockDice>(2, 4));  // Mocked dice roll to move Player 2
        game.playTurn();
        CHECK(player2->getPosition() == 6);
    }

    SUBCASE("Handling bankruptcy") {
        player1->adjustMoney(-1500);
        game.checkBankruptcy();
        CHECK(game.getCurrentPlayer() == player2);  // Player 1 should be removed for being bankrupt
    }
}

TEST_CASE("Property buying") {
    auto player1 = std::make_shared<Player>("Alice", 1500);
    auto player2 = std::make_shared<Player>("Bob", 1500);
    Game game({player1, player2});

    game.addTile(std::make_shared<StreetTile>("Sydney", "Brown", 60, 2));

    // Mocked dice to land on Sydney
    game.setDice(std::make_shared<MockDice>(1, 1));  // Roll to land on Sydney
    game.playTurn();

    CHECK(player1->getMoney() == 1440);  // Money reduced after buying property
    CHECK(player1->getPosition() == 2);  // Landed on tile 2
    CHECK(game.getTile(2)->getOwner() == player1);  // Player 1 owns the tile
}

TEST_CASE("Rent payment") {
    auto player1 = std::make_shared<Player>("Alice", 1500);
    auto player2 = std::make_shared<Player>("Bob", 1500);
    Game game({player1, player2});

    game.addTile(std::make_shared<StreetTile>("Melbourne", "Brown", 60, 4));
    game.setDice(std::make_shared<MockDice>(1, 1));  // Alice buys Melbourne
    game.playTurn();

    game.setDice(std::make_shared<MockDice>(2, 2));  // Bob lands on Melbourne
    game.playTurn();

    CHECK(player1->getMoney() == 1504);  // Alice receives rent
    CHECK(player2->getMoney() == 1496);  // Bob pays rent
}

TEST_CASE("Go to Jail") {
    auto player1 = std::make_shared<Player>("Alice", 1500);
    Game game({player1});

    game.addTile(std::make_shared<GoToJailTile>("Go to Jail"));
    game.addTile(std::make_shared<StreetTile>("Sydney", "Brown", 60, 2));

    game.setDice(std::make_shared<MockDice>(1, 2));  // Mocked dice to land on Go to Jail
    game.playTurn();

    CHECK(player1->isInJail() == true);
    CHECK(player1->getPosition() == 10);  // Jail is typically at position 10
}

TEST_CASE("Player bankruptcy") {
    auto player1 = std::make_shared<Player>("Alice", 50);
    auto player2 = std::make_shared<Player>("Bob", 1500);
    Game game({player1, player2});

    game.addTile(std::make_shared<StreetTile>("Melbourne", "Brown", 60, 4));
    game.setDice(std::make_shared<MockDice>(1, 1));  // Alice buys Melbourne
    game.playTurn();

    game.setDice(std::make_shared<MockDice>(2, 2));  // Bob lands on Melbourne
    game.playTurn();

    CHECK(player1->isBankrupt() == true);  // Alice should be bankrupt
    CHECK(game.checkForWinner() == true);  // Bob wins the game
}
