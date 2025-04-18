
//
// Conways Game of Life 
//

#include <cstdlib>
#include <cmath>
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

    bool gamePaused = false;
    sf::Vector2u cellLookup = {0, 0};
    int generation = 0;
    int neighbors = 0;

    // seed the random number generator
    std::srand(std::time(0));
    

    /// Build the grid

    std::vector<std::vector<int>> lifeGrid(conf::board_size.y + 2, std::vector<int>(conf::board_size.x + 2, 0));
    std::vector<std::vector<int>> lifeGrid_B(conf::board_size.y + 2, std::vector<int>(conf::board_size.x + 2, 0));

    std::vector<sf::RectangleShape> visualGrid(conf::cell_count);
    std::cout << "Built a sf::rectangleShape vector with a length of : " << visualGrid.size() << std::endl;

    sf::Vector2f cellLocation = {0.f, 0.f};
    for (unsigned int i {}; i < conf::cell_count; i++) {

        if (i % (conf::board_size.x) == 0 && i > 0) {
            cellLocation += {0, conf::window_size_f.y / conf::board_size.y};
            cellLocation.x = 0;
            //std::cout << "newline detected at position [" << i << "]\n";
        }
        visualGrid[i].setSize({conf::window_size_f.x / conf::board_size.x, conf::window_size_f.y / conf::board_size.y});
        visualGrid[i].setPosition(cellLocation);
        visualGrid[i].setOutlineColor(grey);
        visualGrid[i].setOutlineThickness(-1.f);

        // randomly determine the life or death of a cell

        if (std::rand() % 2 == 1) {
            lifeGrid[1 + std::floor(i / conf::board_size.x)][1 + (i % conf::board_size.x)] = 1;
            visualGrid[i].setFillColor(sf::Color::White);
        }
        else {
            lifeGrid[1 + std::floor(i / conf::board_size.x)][1 + (i % conf::board_size.x)] = 0;
            visualGrid[i].setFillColor(sf::Color::Black);
        }

        //std::cout << i << " built cell :\nLocation : " << visualGrid[i].getPosition().x << ", " << visualGrid[i].getPosition().y << "\n";

        cellLocation += {conf::window_size_f.x / conf::board_size.x, 0};
    }
    sf::Vector2f size = visualGrid[0].getSize();

    int row = 0;
    int collumn = 0;

    for (unsigned int i {}; i < conf::cell_count; i++) { // print board loop
        cellLookup = {1 + (i / conf::board_size.x), 1 + (i % conf::board_size.x)};

        if (i % conf::board_size.x == 0) {
            std::cout << "\n";
            row++;
        }
        if (lifeGrid[cellLookup.x][cellLookup.y] == 0) {
            std::cout << ". ";
            collumn++;
        }
        else {
            std::cout << "# ";
            collumn++;
        }
    }
    std::cout << std::endl;
    std::cout << "Rows : " << row << "\nCollumns : " << collumn/row << std::endl;
    

    // main program loop
    while (window.isOpen())
    {
        processEvents(window, visualGrid, gamePaused);

        // display the grid
        window.clear();
        for (int i {}; i < conf::cell_count; i++) {

            window.draw(visualGrid[i]);
        }
        window.display();

        // update the cells 
        if (gamePaused == false) {
            for (int i {}; i < conf::cell_count; i++) {
                cellLookup = {1 + (i / conf::board_size.x), 1 + (i % conf::board_size.x)};

                neighbors +=
                    lifeGrid[cellLookup.x - 1][cellLookup.y - 1] // top left
                    + lifeGrid[cellLookup.x][cellLookup.y - 1]     // top center
                    + lifeGrid[cellLookup.x + 1][cellLookup.y - 1] // top right
                    + lifeGrid[cellLookup.x + 1][cellLookup.y]     // middle right
                    + lifeGrid[cellLookup.x + 1][cellLookup.y + 1] // bottom right
                    + lifeGrid[cellLookup.x][cellLookup.y + 1]     // bottom center
                    + lifeGrid[cellLookup.x - 1][cellLookup.y + 1] // bottom left
                    + lifeGrid[cellLookup.x - 1][cellLookup.y];    // middle left

                /*
                top_left = lifeGrid[cellLookup.x - 1][cellLookup.y - 1];
                bottom_left = lifeGrid[cellLookup.x - 1][cellLookup.y + 1];
                top_right = lifeGrid[cellLookup.x + 1][cellLookup.y - 1];
                bottom_right = lifeGrid[cellLookup.x + 1][cellLookup.y + 1];
                neighbors = 0;
                if (!(i % conf::board_size.x == 0)) { // check if possition is NOT on the left wall
                    // check the 3 neighbors to the left
                    neighbors +=
                        lifeGrid[cellLookup.x - 1][cellLookup.y + 1]   // bottom left
                        + lifeGrid[cellLookup.x - 1][cellLookup.y]     // middle left
                        + lifeGrid[cellLookup.x - 1][cellLookup.y - 1] // top left
                        ;
                }

                if (!((i - conf::board_size.x) < 0)) { // check if position is NOT on the top wall
                    // check the 3 neighbors above
                    neighbors +=
                        lifeGrid[cellLookup.x - 1][cellLookup.y - 1]   // top left
                        + lifeGrid[cellLookup.x][cellLookup.y - 1]     // top center
                        + lifeGrid[cellLookup.x + 1][cellLookup.y - 1] // top right
                        ;
                }

                if (!((i + 1) % conf::board_size.x == 0)) { // check if position is NOT on the right wall
                    // check the 3 neighbors to the right
                    neighbors +=
                        lifeGrid[cellLookup.x + 1][cellLookup.y - 1]   // top right
                        + lifeGrid[cellLookup.x + 1][cellLookup.y]     // middle right
                        + lifeGrid[cellLookup.x + 1][cellLookup.y + 1] // bottom right
                        ;
                }

                if (!(i - (conf::board_size.x * conf::board_size.y - conf::board_size.x) >= 0)) { // check if position is NOT on the bottom wall
                    // check the 3 neighbors below
                    neighbors +=
                        lifeGrid[cellLookup.x + 1][cellLookup.y + 1]   // bottom right
                        + lifeGrid[cellLookup.x][cellLookup.y + 1]     // bottom center
                        + lifeGrid[cellLookup.x - 1][cellLookup.y + 1] // bottom left
                        ;
                }
                */
                //std::cout << "board[" << i << "] : " << neighbors << "\n";

                switch (neighbors) {

                case 1: {
                    lifeGrid_B[cellLookup.x][cellLookup.y] = 0;
                    visualGrid[i].setFillColor(sf::Color::Black);
                    break;
                }
                case 2:
                    if (lifeGrid_B[cellLookup.x][cellLookup.y] == 0)
                        break;
                case 3: {
                    lifeGrid_B[cellLookup.x][cellLookup.y] = 1;
                    visualGrid[i].setFillColor(sf::Color::White);
                    break;
                }
                default:
                    lifeGrid_B[cellLookup.x][cellLookup.y] = 0;
                    visualGrid[i].setFillColor(sf::Color::Black);
                }
                neighbors = 0;
            }
        }
        
        lifeGrid = lifeGrid_B;
        generation++;
    }
}