
//
// Conways Game of Life 
//

#include <string>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "configuration.h"
#include "events.h"
#include "cellBoard.h"

struct RenderingCell {
    sf::RectangleShape visualCell;
    sf::Vector2f cellLocation;
    sf::Vector2f cellSize = {conf::window_size_f.x / conf::board_size.x, conf::window_size_f.y / conf::board_size.y};

    RenderingCell(sf::Color bordercolor) {
        visualCell.setSize(cellSize);
        visualCell.setOutlineColor(bordercolor);
        visualCell.setOutlineThickness(-1.f);
    }

};
void renderCells(sf::RenderWindow& window, CellBoard& GameOfLife, RenderingCell& renderingCell) {
    renderingCell.cellLocation = {0.f, 0.f};

    for (int index : GameOfLife.getAliveCells()) {

        //find the pixel location of the cell based off of its index in the grid
        renderingCell.cellLocation = {static_cast<float>(index % (conf::board_size.x)),floor(static_cast<float>(index - conf::board_size.x) / conf::board_size.x) + 1};

        renderingCell.visualCell.setPosition({renderingCell.cellSize.x * renderingCell.cellLocation.x, renderingCell.cellSize.y * renderingCell.cellLocation.y});

        window.draw(renderingCell.visualCell);
        renderingCell.cellLocation += {conf::window_size_f.x / conf::board_size.x, 0};
    }
}

int main()
{

    // setup rendering windows
    sf::RenderWindow window(sf::VideoMode(conf::window_size), conf::window_title);
    window.setFramerateLimit(conf::max_framerate);
    
    std::cout << 
        "\nPrimary window created" <<
        "\nTitle : " << conf::window_title <<
        "\nDimensions : " << conf::window_size.x <<", " << conf::window_size.y <<
        "\nMax Framerate : " << conf::max_framerate << 
        std::endl;

    sf::RenderWindow statsWindow(sf::VideoMode(conf::stats_window_size), "Game Statistics");
    statsWindow.setFramerateLimit(conf::max_framerate);
    statsWindow.setPosition({window.getPosition().x - static_cast<int>(conf::stats_window_size.x), window.getPosition().y});
    sf::Font statsFont("RobotoMono-VariableFont_wght.ttf");

    std::cout <<
        "\nStats window created" <<
        "\nTitle : " << conf::stats_window_title <<
        "\nDimensions : " << conf::stats_window_size.x << ", " << conf::stats_window_size.y <<
        "\nMax Framerate : " << conf::max_framerate <<
        std::endl;

    sf::Clock statsClock;
    sf::Clock fpsClock;
    sf::Time fpsTime;

    sf::Text generationCount(statsFont);
    generationCount.setCharacterSize(conf::text_size);

    sf::Text populationCount(statsFont);
    populationCount.setCharacterSize(conf::text_size);
    populationCount.setPosition({0, conf::text_size});

    sf::Text fpsText(statsFont);
    fpsText.setCharacterSize(conf::text_size);
    fpsText.setPosition({0, conf::text_size * 2});

    sf::Text computeTime(statsFont);
    computeTime.setCharacterSize(conf::text_size);
    computeTime.setPosition({0, conf::text_size * 3});

    sf::Text renderTime(statsFont);
    renderTime.setCharacterSize(conf::text_size);
    renderTime.setPosition({0, conf::text_size * 4});

    window.requestFocus();

    sf::Color gray = {54, 69, 79}; // charcoal gray

    SimState simState {};

    /// Build the Board ///

    statsClock.restart();
    CellBoard GameOfLife(conf::board_size);
    GameOfLife.generateBoard();
    std::vector<std::vector<int>> lifeBoard_T = GameOfLife.getBoard();
    int generation = 0;

    std::cout << "\nBoard with " << conf::cell_count << " cells generated in " << statsClock.getElapsedTime().asSeconds() << " seconds" << std::endl;


    /// Render the Board ///

    RenderingCell renderingCell(gray);

    // main program loop
    while (window.isOpen()){

        processEvents(window, simState);

        if (simState.reSeed) {
            GameOfLife.generateBoard();
            simState.reSeed = false;
            window.display();
        }
        else if (simState.clear) {
            GameOfLife.clearBoard();
            simState.clear = false;
            window.display();
        }

        if (simState.mouseHeld) {
            std::cout <<
                "\n Mouse X : " << sf::Mouse::getPosition(window).x << " " << conf::board_size.x << " " << sf::Mouse::getPosition(window).x / conf::board_size.x <<
                "\n Mouse Y : " << sf::Mouse::getPosition(window).y << std::endl;

            // bruteforce way of doing this. I know that there is a simpler way, but I have not yet bothered to work it out.
            for (unsigned int x = 0; x < conf::board_size.x; x++) {
                for (unsigned int y = 0; y < conf::board_size.y; y++) {
                    if ((x * renderingCell.cellSize.x <= sf::Mouse::getPosition(window).x &&
                        sf::Mouse::getPosition(window).x <= (x + 1) * renderingCell.cellSize.x) && 
                        (y * renderingCell.cellSize.y <= sf::Mouse::getPosition(window).y &&
                        sf::Mouse::getPosition(window).y <= (y + 1) * renderingCell.cellSize.y)) {

                        std::cout << "hit cell at location (" << x << ", " << y << ")" << std::endl;
                        GameOfLife.birthCell(x, y);
                        GameOfLife.useBuffer();

                        window.clear();
                        renderCells(window, GameOfLife, renderingCell);
                        window.display();
                    }
                }
            }
        }

        // update the cells 
        if (!simState.simPaused) {

            // Display the grid
            window.clear();

            statsClock.restart();

            renderCells(window, GameOfLife, renderingCell);

            window.display();
            renderTime.setString("renderTime : " + std::to_string(static_cast<float>(statsClock.getElapsedTime().asMicroseconds()) / 1000).substr(0, 5) + " ms");
            statsClock.restart();
            
            GameOfLife.updateCells();
            generationCount.setString("generation : " + std::to_string(GameOfLife.getGeneration()));
            populationCount.setString("population : " + std::to_string(GameOfLife.getPopulation()));

            computeTime.setString("computeTime : " + std::to_string(static_cast<float>(statsClock.getElapsedTime().asMicroseconds()) / 1000).substr(0, 5) + " ms");

            fpsTime = fpsClock.restart();
            fpsText.setString("fps : " + std::to_string(static_cast<int>(1 / fpsTime.asSeconds())) + " / " + std::to_string(static_cast<float>(fpsTime.asMicroseconds()) / 1000).substr(0, 5) + " ms");

            // Display the stats
            statsWindow.clear();
            statsWindow.draw(generationCount);
            statsWindow.draw(populationCount);
            statsWindow.draw(fpsText);
            statsWindow.draw(computeTime);
            statsWindow.draw(renderTime);
            statsWindow.display();

            
            if (conf::printLog) {
                lifeBoard_T = GameOfLife.getBoard();
                std::cout << "\n" << std::endl;
                for (unsigned int i = 0; i < conf::cell_count; i++) {
                    std::cout << lifeBoard_T[GameOfLife.cellLookup(i).x][GameOfLife.cellLookup(i).y] << " ";
                    if ((i % conf::board_size.x) == 0 && i > 5)
                        std::cout << std::endl;
                } // generation check - print initial state to console
            }            
            
            if (simState.stepFrame) {
                simState.simPaused = true;
            }
        }
    }
}

