
#include <SFML/System.hpp>

namespace conf {

	// Window Config
	const sf::Vector2u window_size = {1080, 1080};
	const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
	const int max_framerate = 144;
	

	// Board Config
	const sf::Vector2u board_size = {100, 100};
	const int cell_count = (board_size.x * board_size.y);

}
