
#include <SFML/System.hpp>

namespace conf {

	// Main Window Config
	const sf::Vector2u window_size = {720, 720};
	const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
	const int max_framerate = 24;

	// Stats Window Config

	const sf::Vector2u stats_window_size {200, 200};
	const unsigned int text_size = 15;
	

	// Board Config
	const sf::Vector2u board_size = {60, 60};
	const unsigned int cell_count = (board_size.x * board_size.y);

}
