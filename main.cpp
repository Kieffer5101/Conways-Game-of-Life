
//
// Conways Game of Life 
//

/* Known Bugs
 
 * placing a cell while paused skips to the next frame
 * killing a cell while paused does not remove it from the screen instantly
 * clearing the board will render the previous generation of cells for a frame (easiest seen when paused)
 *

*/

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

    RenderingCell(sf::Color bordercolor) { // renderingCell constructor
        visualCell.setSize(cellSize);
        visualCell.setOutlineColor(bordercolor);
        visualCell.setOutlineThickness(-1.f);
    }

};

struct StatsText {

    sf::Font statsFont;
    sf::Text generationCount;
    sf::Text populationCount;
    sf::Text fpsText;
    sf::Text computeTime;
    sf::Text renderTime;
    sf::Text controlsInfo;

    StatsText(sf::Font statsFont) :
        statsFont(statsFont),
        generationCount(statsFont),
        populationCount(statsFont),
        fpsText(statsFont),
        computeTime(statsFont),
        renderTime(statsFont),
        controlsInfo(statsFont) {
        
        generationCount.setCharacterSize(conf::text_size);

        populationCount.setCharacterSize(conf::text_size);
        populationCount.setPosition({0, conf::text_size});
        
        fpsText.setCharacterSize(conf::text_size);
        fpsText.setPosition({0, conf::text_size * 2});
        
        computeTime.setCharacterSize(conf::text_size);
        computeTime.setPosition({0, conf::text_size * 3});
        
        renderTime.setCharacterSize(conf::text_size);
        renderTime.setPosition({0, conf::text_size * 4});

        controlsInfo.setCharacterSize(conf::text_size);
        controlsInfo.setPosition({0, conf::text_size * 5});

        controlsInfo.setString("\n[Space] - Pause / Play" 
            "\n[N] - Step Frame"
            "\n[C] - Clear"
            "\n[R] - Reseed"
            "\n[S] - Open Stats Window"
            "\n[P] - Toggle Pause on press");
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

void createStatsWindow(sf::Window& window, sf::Window& statsWindow, StatsText& statsText) {

    statsWindow.create(sf::VideoMode(conf::stats_window_size), "Game Statistics");
    statsWindow.setFramerateLimit(conf::max_framerate);
    statsWindow.setPosition({window.getPosition().x - static_cast<int>(conf::stats_window_size.x), window.getPosition().y});

    statsText.generationCount.setFont(statsText.statsFont);
    statsText.populationCount.setFont(statsText.statsFont);
    statsText.fpsText.setFont(statsText.statsFont);
    statsText.computeTime.setFont(statsText.statsFont);
    statsText.renderTime.setFont(statsText.statsFont);
    statsText.controlsInfo.setFont(statsText.statsFont);
}

int main(){

    // setup rendering windows
    sf::RenderWindow window(sf::VideoMode(conf::window_size), conf::window_title);
    window.setFramerateLimit(conf::max_framerate);
    
    std::cout << 
        "\nPrimary window created" <<
        "\nTitle : " << conf::window_title <<
        "\nDimensions : " << conf::window_size.x <<", " << conf::window_size.y <<
        "\nMax Framerate : " << conf::max_framerate << 
        std::endl;

    sf::Font statsFont("RobotoMono-VariableFont_wght.ttf");
    StatsText statsText(statsFont);

    sf::RenderWindow statsWindow(sf::VideoMode(conf::stats_window_size), "Game Statistics");
    createStatsWindow(window, statsWindow, statsText);
    
    window.requestFocus();

    sf::Clock statsClock;
    sf::Clock fpsClock;
    sf::Time fpsTime;

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

        processEvents(window, statsWindow, simState);

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
        
        if (simState.buildStatsWindow) {
            createStatsWindow(window, statsWindow, statsText);
            simState.buildStatsWindow = false;
        }

        // Spawn new cells at the location of the cursor
        if (simState.mouseHeld) {
            
            // brute force way of doing this. I know that there is a simpler way, but I have not yet bothered to work it out.
            for (unsigned int x = 0; x < conf::board_size.x; x++) {
                for (unsigned int y = 0; y < conf::board_size.y; y++) {
                    if ((x * renderingCell.cellSize.x <= sf::Mouse::getPosition(window).x &&
                        sf::Mouse::getPosition(window).x <= (x + 1) * renderingCell.cellSize.x) && 
                        (y * renderingCell.cellSize.y <= sf::Mouse::getPosition(window).y &&
                        sf::Mouse::getPosition(window).y <= (y + 1) * renderingCell.cellSize.y)) {

                        switch (simState.mouseKey) {
                        case 1: {
                             std::cout << "birthed cell at location (" << x << ", " << y << ")" << std::endl;
                             GameOfLife.birthCell(x, y);
                             break;
                        }
                        case 2: {
                            std::cout << "killed cell at location (" << x << ", " << y << ")" << std::endl;
                            GameOfLife.killCell(x, y);
                            break;
                        }
                        }
                        
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
            statsText.renderTime.setString("renderTime : " + std::to_string(static_cast<float>(statsClock.getElapsedTime().asMicroseconds()) / 1000).substr(0, 5) + " ms");
            statsClock.restart();
            
            GameOfLife.updateCells();

            statsText.generationCount.setString("generation : " + std::to_string(GameOfLife.getGeneration()));
            statsText.populationCount.setString("population : " + std::to_string(GameOfLife.getPopulation()));

            statsText.computeTime.setString("computeTime : " + std::to_string(static_cast<float>(statsClock.getElapsedTime().asMicroseconds()) / 1000).substr(0, 5) + " ms");

            fpsTime = fpsClock.restart();
            statsText.fpsText.setString("fps : " + std::to_string(static_cast<int>(1 / fpsTime.asSeconds())) + " / " + std::to_string(static_cast<float>(fpsTime.asMicroseconds()) / 1000).substr(0, 5) + " ms");

            // Display the stats
            statsWindow.clear();
            statsWindow.draw(statsText.generationCount);
            statsWindow.draw(statsText.populationCount);
            statsWindow.draw(statsText.fpsText);
            statsWindow.draw(statsText.computeTime);
            statsWindow.draw(statsText.renderTime);
            statsWindow.draw(statsText.controlsInfo);
            statsWindow.display();           
            
            if (simState.stepFrame) {
                simState.simPaused = true;
            }
        }
    }
}