
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "events.h"


int pressCount = 0;

void processEvents(sf::Window& window, std::vector<sf::RectangleShape>& grid) {

    while (const auto event = window.pollEvent()){

        if (event->is<sf::Event::Closed>())
            window.close();

        else if (event->is<sf::Event::KeyPressed>()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }
        else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePress->button == sf::Mouse::Button::Left) {
                std::cout << "The left button was pressed " << ++pressCount << std::endl;
                std::cout << "mouse x : " << mousePress->position.x << std::endl;
                std::cout << "mouse y : " << mousePress->position.y << std::endl;
            }
        }
    }
}