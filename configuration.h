#pragma once

#include <string>
#include <SFML/System.hpp>

struct SimState {
	bool buildStatsWindow = false;
	bool simPaused = false;
	bool stepFrame = false;
    bool mouseHeld = false;
	bool pauseOnHold = true;
	bool reSeed = false;
	bool clear = false;
	int mouseKey = 0;
	int pressCount = 0;
};

namespace conf {

	const bool printLog = false;

	// primary Window Config
    const std::string window_title= "Conways Game of Life";
	const sf::Vector2u window_size = {950, 950};
	const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
	const int max_framerate = 100;


	// Stats Window Config
    const std::string stats_window_title = "Simulation statistics";
	const sf::Vector2u stats_window_size {300, 250};
	const unsigned int text_size = 15;
	

	// Board Config
	const sf::Vector2i board_size = {200,200};
	const unsigned int cell_count = (board_size.x * board_size.y);

}  