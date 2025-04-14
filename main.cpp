
//
// Conways Game of Life 
//

#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "configuration.h"
#include "events.h"




int main()
{
    sf::RenderWindow window(sf::VideoMode(conf::window_size), "Conways Game of Life");
    window.setFramerateLimit(conf::max_framerate);

    // charcoal gray
    sf::Color grey = {54, 69, 79};

    // seed the random number generator
    std::srand(std::time(0));

    /// Build the grid

    std::vector<sf::RectangleShape> boardGrid(conf::cell_count);
    std::cout << "Built a sf::rectangleShape vector with a length of : " << boardGrid.size() << std::endl;

    sf::Vector2f location = {0.f, 0.f};
    for (int i {}; i < conf::cell_count; i++) {

        if (i % (conf::board_size.x) == 0 && i > 0) {
            location += {0, conf::window_size_f.y / conf::board_size.y};
            location.x = 0;
            //std::cout << "newline detected at position [" << i << "]\n";
        }
        boardGrid[i].setSize({conf::window_size_f.x / conf::board_size.x, conf::window_size_f.y / conf::board_size.y});
        boardGrid[i].setPosition(location);
        boardGrid[i].setOutlineColor(grey);
        boardGrid[i].setOutlineThickness(-1.f);

        // randomly determine the life or death of a cell
        if (std::rand() % 2 == 1) 
            boardGrid[i].setFillColor(sf::Color::White);
        else
            boardGrid[i].setFillColor(sf::Color::Black);


        //std::cout << i << " built cell :\nLocation : " << boardGrid[i].getPosition().x << ", " << boardGrid[i].getPosition().y << "\n";

        location += {conf::window_size_f.x / conf::board_size.x, 0};
    }
    sf::Vector2f size = boardGrid[0].getSize();

    boardGrid[24].setFillColor(sf::Color::White);
    

    // main program loop
    while (window.isOpen())
    {
        processEvents(window, boardGrid);

        window.clear();

        // display the grid
        for (int i {}; i < conf::cell_count; i++) {

            window.draw(boardGrid[i]);
        }

        window.display();
    }
}