#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "game.hpp"  // Game class and other headers you have for the board and players

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Monopoly Board");

    // Create the game instance
    std::vector<std::shared_ptr<Player>> players = {std::make_shared<Player>("Player 1"), std::make_shared<Player>("Player 2")};
    Game game(players);

    // Main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        game.drawBoard(window);  // Use the updated drawBoard method
        window.display();
    }

    return 0;
}
