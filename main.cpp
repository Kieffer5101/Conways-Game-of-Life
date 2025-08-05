
//
// Conways Game of Life 
//

#include <thread>
#include <string>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "configuration.h"
#include "events.h"

void renderSection(sf::Window& window, std::vector<int> cells) {

}

int main()
{
    sf::RenderWindow window(sf::VideoMode(conf::window_size), "Conways Game of Life");
    window.setFramerateLimit(conf::max_framerate);

    sf::RenderWindow statsWindow(sf::VideoMode(conf::stats_window_size), "Game Statistics");
    statsWindow.setFramerateLimit(conf::max_framerate);
    statsWindow.setPosition({window.getPosition().x - static_cast<int>(conf::stats_window_size.x), window.getPosition().y});
    sf::Font statsFont("RobotoMono-VariableFont_wght.ttf"); 
    
    sf::Clock statsClock;
    sf::Clock fpsClock;
    sf::Time fpsTime;

    sf::Text generationCount(statsFont);
    generationCount.setCharacterSize(conf::text_size);

    sf::Text fpsText(statsFont);
    fpsText.setCharacterSize(conf::text_size);
    fpsText.setPosition({0, conf::text_size});

    sf::Text computeTime(statsFont);
    computeTime.setCharacterSize(conf::text_size);
    computeTime.setPosition({0, conf::text_size * 2});

    sf::Text renderTime(statsFont);
    renderTime.setCharacterSize(conf::text_size);
    renderTime.setPosition({0, conf::text_size * 3});

    window.requestFocus();

    // charcoal gray
    sf::Color grey = {54, 69, 79};

    bool gamePaused = false;
    sf::Vector2u cellLookup = {0, 0};
    int generation = 0;
    int neighbors = 0;
    int before = 0;

    // seed the random number generator
    std::srand(std::time(0));

    /// Build the grid

    std::vector<int> updatedCells;

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
            updatedCells.push_back(i);
        }
        else {
            lifeGrid[1 + std::floor(i / conf::board_size.x)][1 + (i % conf::board_size.x)] = 0;
            visualGrid[i].setFillColor(sf::Color::Black);
        }
        

        //std::cout << i << " built cell :\nLocation : " << visualGrid[i].getPosition().x << ", " << visualGrid[i].getPosition().y << "\n";

        cellLocation += {conf::window_size_f.x / conf::board_size.x, 0};
    }

    // main program loop
    while (window.isOpen())
    {
        processEvents(window, visualGrid, gamePaused);

        // update the cells 
        if (gamePaused == false) {

            // Display the grid
            window.clear();

            statsClock.restart();

            for (int location : updatedCells) {

                window.draw(visualGrid[location]);
            }

            window.display();
            renderTime.setString("renderTime : " + std::to_string(static_cast<float>(statsClock.getElapsedTime().asMicroseconds()) / 1000).substr(0, 5) + " ms");


            updatedCells.clear();
            statsClock.restart();

            for (int i {}; i < conf::cell_count; i++) {
                cellLookup = {1 + (i / conf::board_size.x), 1 + (i % conf::board_size.x)};

                neighbors +=
                    lifeGrid[cellLookup.x - 1][cellLookup.y - 1]   // top left
                    + lifeGrid[cellLookup.x][cellLookup.y - 1]     // top center
                    + lifeGrid[cellLookup.x + 1][cellLookup.y - 1] // top right
                    + lifeGrid[cellLookup.x + 1][cellLookup.y]     // middle right
                    + lifeGrid[cellLookup.x + 1][cellLookup.y + 1] // bottom right
                    + lifeGrid[cellLookup.x][cellLookup.y + 1]     // bottom center
                    + lifeGrid[cellLookup.x - 1][cellLookup.y + 1] // bottom left
                    + lifeGrid[cellLookup.x - 1][cellLookup.y];    // middle left

                before = lifeGrid[cellLookup.x][cellLookup.y];

                switch (neighbors) {

                case 1: {
                    lifeGrid_B[cellLookup.x][cellLookup.y] = 0;
                    visualGrid[i].setFillColor(sf::Color::Black);
                    visualGrid[i].setOutlineThickness(0);
                    break;
                }
                case 2:
                    if (lifeGrid_B[cellLookup.x][cellLookup.y] == 0)
                        break;
                    [[fallthrough]];
                case 3: {
                    lifeGrid_B[cellLookup.x][cellLookup.y] = 1;
                    visualGrid[i].setFillColor(sf::Color::White);
                    visualGrid[i].setOutlineThickness(-0.5f);
                    break;
                }
                default:
                    lifeGrid_B[cellLookup.x][cellLookup.y] = 0;
                    visualGrid[i].setFillColor(sf::Color::Black);
                    visualGrid[i].setOutlineThickness(0);
                }
                neighbors = 0;
                
                if (lifeGrid_B[cellLookup.x][cellLookup.y] != before || lifeGrid_B[cellLookup.x][cellLookup.y] == 1)
                    updatedCells.push_back(i);
            }
            lifeGrid = lifeGrid_B;

            generationCount.setString("generation : " + std::to_string(generation++));
            computeTime.setString("computeTime : " + std::to_string(static_cast<float>(statsClock.getElapsedTime().asMicroseconds()) / 1000).substr(0, 5) + " ms");            
        }
        
        fpsTime = fpsClock.restart();
        fpsText.setString("fps : " + std::to_string(static_cast<int>(1 / fpsTime.asSeconds())) + " / " + std::to_string(static_cast<float>(fpsTime.asMicroseconds()) / 1000).substr(0, 5) + " ms");
        
        // Display the stats
        statsWindow.clear();
        statsWindow.draw(generationCount);
        statsWindow.draw(fpsText);
        statsWindow.draw(computeTime);
        statsWindow.draw(renderTime);
        statsWindow.display();
    }
}