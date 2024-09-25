#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "game.hpp"

// Assuming you have the Player class and the Game class with drawPlayers implemented
int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Monopoly Board Test");

    
    

    // Create test players
    std::vector<std::shared_ptr<Player>> players;

    Game game(players);
    
    // Adding 4 players with different colors and positions
    auto player1 = std::make_shared<Player>(sf::Color::Red, 5);
    auto player2 = std::make_shared<Player>(sf::Color::Blue, 5);
    auto player3 = std::make_shared<Player>(sf::Color::Green, 5);
    auto player4 = std::make_shared<Player>(sf::Color::Yellow, 20);
    
    
    players.push_back(player1);
    players.push_back(player2);
    players.push_back(player3);
    players.push_back(player4);

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the board
        game.drawBoard(window);

        // Draw the players on the board
        game.drawPlayers(window, players);

        // Display the window
        window.display();
    }

    return 0;
}
