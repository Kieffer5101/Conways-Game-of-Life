
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "events.h"


void processEvents(sf::Window& window, SimState& simState) {

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

                // std::cout << "stepFrame : " << simState.stepFrame << std::endl;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                simState.reSeed = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
                simState.clear = true;
            }
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                simState.mouseHeld = true;
            }
        }
        else if (event->is<sf::Event::MouseButtonReleased>()) {
            simState.mouseHeld = false;
        }
    }
}