
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "events.h"


void processEvents(sf::Window& window, sf::Window& statsWindow, SimState& simState) {

    std::cout << std::boolalpha;

    while (const auto event = window.pollEvent()){
        
        if (event->is<sf::Event::Closed>())
            window.close();

        else if (event->is<sf::Event::KeyPressed>()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                simState.simPaused = !simState.simPaused;
                simState.stepFrame = false;
                std::cout << "\nsim paused : " << simState.simPaused << std::endl;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N) && simState.simPaused) {
                simState.simPaused = false;
                simState.stepFrame = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                simState.reSeed = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
                simState.clear = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                simState.buildStatsWindow = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
                simState.pauseOnHold = !simState.pauseOnHold;
            }
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (simState.pauseOnHold)
                    simState.simPaused = true;
                simState.mouseHeld = true;
                simState.mouseKey = 1;
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                if (simState.pauseOnHold)
                    simState.simPaused = true;
                simState.mouseHeld = true;
                simState.mouseKey = 2;
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle)) {
                std::cout <<
                    "\n Mouse X : " << sf::Mouse::getPosition(window).x <<
                    "\n Mouse Y : " << sf::Mouse::getPosition(window).y << std::endl;
            }
        }
        else if (event->is<sf::Event::MouseButtonReleased>()) {
            simState.mouseHeld = false;
            // simState.simPaused = false;
            simState.mouseKey = 0;
        }
    }

    while (const auto statsEvent = statsWindow.pollEvent()) {

        if (statsEvent->is<sf::Event::Closed>())
            statsWindow.close();
    }
}