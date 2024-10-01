#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "player.hpp"
#include "streetTile.hpp"
#include "railroadTile.hpp"
#include "specialTiles.hpp"
#include "board.hpp"
#include "game.hpp"
#include "dice.hpp"
#include "cards.hpp"
#include <SFML/System.hpp>

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
        player.move(5);  // Moving 5 steps
        CHECK(player.getPosition() == 5);

        player.move(9); // Wrapping around the board
        CHECK(player.getPosition() == 14);  
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

TEST_CASE("Railroad Rent payment") {
    auto player1 = std::make_shared<Player>("Alice", 1500);
    auto player2 = std::make_shared<Player>("Bob", 1500);
    Game game({player1, player2});

    CHECK(player1->getMoney() == 1500);  // Alice receives rent
    CHECK(player2->getMoney() == 1500); 
    player1->setPosition(10);
    game.setDice(std::make_shared<MockDice>(3, 2));  // Alice buys Reading Railroad(200$)
    game.playTurn();

    player2->setPosition(10);   
    game.setDice(std::make_shared<MockDice>(3, 2));  // Bob lands on Reading Railroad(pay 50 to Alice)
    game.playTurn();

    CHECK(player1->getMoney() == 1350);  // Alice receives rent
    CHECK(player2->getMoney() == 1450);  // Bob pays rent
}


// Test cases for Game class
TEST_CASE("Game class tests") {
    auto player1 = std::make_shared<Player>("Alice", 1500);
    auto player2 = std::make_shared<Player>("Bob", 1500);
    Game game({player1, player2});


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

   

    // Mocked dice to land on St. Charles Place
    game.setDice(std::make_shared<MockDice>(5, 6));  // Roll to land on St. Charles Place
    game.playTurn();

    CHECK(player1->getMoney() == 1360);  // Money reduced after buying property
    CHECK(player1->getPosition() == 11);  // Landed on tile 9
    CHECK(game.getTile(11)->getOwner() == player1);  // Player 1 owns the tile
}



TEST_CASE("Go to Jail") {
    auto player1 = std::make_shared<Player>("Alice", 1500);
    auto player2 = std::make_shared<Player>("Bob", 1500);
    Game game({player1, player2});

    // Manually set player position to tile 25 (near jail)
    player1->setPosition(25);

    // Mock dice roll to move the player 5 tiles, landing them on tile 30 (jail)
    game.setDice(std::make_shared<MockDice>(2, 3));  // This totals 5, moving Alice to tile 30 (jail)
    game.playTurn();

    // Check that the player is now in jail and has moved to tile 30
    CHECK(player1->isInJail() == true);
    CHECK(player1->getPosition() == 10);  // Jail is at position 30
}


TEST_CASE("Player bankruptcy") {
    auto player1 = std::make_shared<Player>("Alice", 6);
    auto player2 = std::make_shared<Player>("Bob", 1500);
    Game game({player1, player2});

   
    game.setDice(std::make_shared<MockDice>(3, 5));  // Alice buys Melbourne
    game.playTurn();

    game.setDice(std::make_shared<MockDice>(6, 2));  // Bob lands on Melbourne
    game.playTurn();

    
    CHECK(player1->isBankrupt() == true);  // Alice should be bankrupt
    CHECK(game.checkForWinner() == true);  // Bob wins the game
}


TEST_CASE("UtilityTile rent calculation") {
    auto player6 = std::make_shared<Player>("Alice", 1500);
    auto player7 = std::make_shared<Player>("Bob", 1500);
    Game game({player6, player7});

    game.setDice(std::make_shared<MockDice>(5, 7));  // Mocking dice roll
    game.playTurn();  // Player 1 buys Utility

    CHECK(game.getTile(12)->getOwner() == player6);

    game.setDice(std::make_shared<MockDice>(8, 4));  // Player 2 lands on Utility
    game.playTurn();  // Player 2 pays rent

    CHECK(player6->getMoney() == 1398);  // Rent = 4 * dice roll. Alice paid 150 for the utility, and get paid 48 from Bob
    CHECK(player7->getMoney() == 1452);  // Bob paid 48 to Alice
}


TEST_CASE("Prevent property re-buying") {
    SUBCASE("No re-buy after property is bought") {
        auto player4 = std::make_shared<Player>("Alice", 1500);
        auto player5 = std::make_shared<Player>("Bob", 1500);
        Game game1({player4, player5});

        // Alice buys St. James Place (position 16)
        game1.setDice(std::make_shared<MockDice>(10, 6));  
        game1.playTurn();
        CHECK(game1.getTile(16)->getOwner() == player4); // Alice pays 180 to buy the street.

        // Bob lands on St. James Place and pays rent
        game1.setDice(std::make_shared<MockDice>(9, 7));  
        game1.playTurn();
        CHECK(game1.getTile(16)->getOwner() == player4);  // Ownership remains Alice's
        CHECK(player5->getMoney() == 1486);  // Bob pays 14 rent to Alice
        CHECK(player4->getMoney() == 1334);  // Alice receives rent 14
    }
}

TEST_CASE("Player attempts to buy property with insufficient funds") {
    auto player = std::make_shared<Player>("Bob", 50);  // Player with only 50$
    auto player2 = std::make_shared<Player>("Alice", 1500);
    Game game({player, player2});

    // Set dice for Bob to land on Mediterranean Avenue (position 1, cost 60$)
    game.setDice(std::make_shared<MockDice>(10, 9));  // Lands on Mediterranean Ave
    game.playTurn();
    CHECK(game.getTile(1)->getOwner() == nullptr);  // Property should remain unowned
    CHECK(player->getMoney() == 50);  // Bob's money remains 50 since he can't afford the property
}

TEST_CASE("Chance Cards: Take a Trip to Reading Railroad") {
    auto player = std::make_shared<Player>("Player 1", 1500);
    auto owner = std::make_shared<Player>("Owner", 1500);
    Game game({player, owner});
    Board& board = Board::getInstance();

    auto readingRailroad = std::dynamic_pointer_cast<RailroadTile>(board.getTile(5));

    SUBCASE("Landing on unowned Reading Railroad") {
        player->setPosition(2);  // Start at position 2
        readingRailroad->setOwner(nullptr);
        auto card = std::make_shared<TripToReadingRailroadCard>();
        player->handleChanceCard(card, game);

        CHECK(player->getPosition() == 5);  // Moves to Reading Railroad
        CHECK(player->getMoney() == 1300);  // Deducted $200 for the purchase
    }

    SUBCASE("Landing on owned Reading Railroad") {
        player->setPosition(2);
        readingRailroad->setOwner(owner);  // Set owner
        auto card = std::make_shared<TripToReadingRailroadCard>();
        player->handleChanceCard(card, game);

        CHECK(player->getPosition() == 5);   // Correct position
        CHECK(player->getMoney() == 1450);   // Paid $50 rent
        CHECK(owner->getMoney() == 1550);    // Owner received $50
    }
}


TEST_CASE("Chance Cards: Advance to Nearest Utility") {
    auto player10 = std::make_shared<Player>("Player 1", 1500);
    auto owner10 = std::make_shared<Player>("Owner", 1500);
    Game game10({player10, owner10});
    Board& board = Board::getInstance();

    auto electricCompany = std::dynamic_pointer_cast<UtilityTile>(board.getTile(12));
    auto waterWorks = std::dynamic_pointer_cast<UtilityTile>(board.getTile(28));

    SUBCASE("Moving to nearest unowned utility") {
        player10->setPosition(10);
        electricCompany->setOwner(nullptr);
        auto card = std::make_shared<AdvanceToNearestUtilityCard>();
        player10->handleChanceCard(card, game10);

        CHECK(player10->getPosition() == 12);  // Correct position
        CHECK(player10->getMoney() == 1350);  // Deducted $150 for the purchase
    }

    SUBCASE("Landing on owned utility and paying rent") {
        player10->setPosition(22);
        waterWorks->setOwner(owner10);  // Set owner
        auto card = std::make_shared<AdvanceToNearestUtilityCard>();
        game10.setDice(std::make_shared<MockDice>(3, 4));  // Set dice roll
        player10->handleChanceCard(card, game10);

        CHECK(player10->getPosition() == 28);  // Correct position
       
    }
}

TEST_CASE("Chance Cards: Advance to Nearest Railroad") {
    auto player = std::make_shared<Player>("Player 1", 1500);
    auto owner = std::make_shared<Player>("Owner", 1500);
    Game game({player, owner});
    Board& board = Board::getInstance();

    auto readingRailroad = std::dynamic_pointer_cast<RailroadTile>(board.getTile(5));
    auto pennsylvaniaRailroad = std::dynamic_pointer_cast<RailroadTile>(board.getTile(15));

    SUBCASE("Moving to nearest unowned railroad") {
        player->setPosition(12);
        readingRailroad->setOwner(nullptr);
        auto card = std::make_shared<AdvanceToNearestRailroadCard>();
        player->handleChanceCard(card, game);

        CHECK(player->getPosition() == 15);
        CHECK(player->getMoney() == 1400);  // Deducted $200 for purchase
    }

    SUBCASE("Landing on owned railroad and paying rent") {
        player->setPosition(8);
        pennsylvaniaRailroad->setOwner(owner);
        auto card = std::make_shared<AdvanceToNearestRailroadCard>();
        player->handleChanceCard(card, game);

        CHECK(player->getPosition() == 15);  // Correct position
        CHECK(player->getMoney() == 1400);   // Paid $100 rent
        CHECK(owner->getMoney() == 1600);    // Owner received $100
    }
}


TEST_CASE("Community Chest Cards: Get Out of Jail Free Card") {
    auto player = std::make_shared<Player>("Player 1", 1500);
    Game game({player});
    auto card = std::make_shared<GetOutOfJailFreeCard>();

    player->handleCommunityChestCard(card, game);
    CHECK(player->hasGetOutOfJailFreeCard() == true);
}

TEST_CASE("Community Chest Cards: Advance to Go") {
    auto player = std::make_shared<Player>("Player 1", 1500);
    Game game({player});
    Board& board = Board::getInstance();

    auto card = std::make_shared<AdvanceToGoCard>();
    player->setPosition(15);  // Set player far from Go
    player->handleCommunityChestCard(card, game);

    CHECK(player->getPosition() == 0);  // Moves to Go
    CHECK(player->getMoney() == 1700);  // Collected $200
}



TEST_CASE("Community Chest Cards: Go to Jail") {
    auto player = std::make_shared<Player>("Player 1", 1500);
    Game game({player});
    Board& board = Board::getInstance();

    auto card = std::make_shared<GoToJailCard>();
    player->handleCommunityChestCard(card, game);

    CHECK(player->getPosition() == 10);  // Jail position is at 10
    CHECK(player->isInJail() == true);
}



// Test cases for Board class
TEST_CASE("Board class tests") {    
    Board& board = Board::getInstance();

    SUBCASE("Checking board initialization") {

        CHECK(board.getTileCount() == 40);
        CHECK(board.getTile(0)->getName() == "Go");
        CHECK(board.getTile(1)->getName() == "Mediterranean Ave");
        CHECK(board.getTile(5)->getName() == "Reading Railroad");
        CHECK(board.getTile(13)->getName() == "States Ave");
        CHECK(board.getTile(19)->getName() == "New York Ave");
        CHECK(board.getTile(36)->getName() == "Chance");
    }

    SUBCASE("Removing tiles from the board") {
        
        board.removeTile(1);
        CHECK(board.getTileCount() == 39);
        CHECK(board.getTile(1)->getName() == "Community Chest");
    }

    SUBCASE("Fetching non-existent tiles") {
        auto tile = board.getTile(40);  // Out of bounds
        CHECK(tile == nullptr);
    }
}
