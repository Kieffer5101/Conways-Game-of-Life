
#include <SFML/System.hpp>

namespace conf {

	// Window Config
	const sf::Vector2u window_size = {720, 720};
	const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
	const int max_framerate = 144;
	

	// Board Config
	const sf::Vector2u board_size = {100, 100};
	const int cell_count = (board_size.x * board_size.y);
	//const int cell_count_L = conf::cell_count + (2 * conf::board_size.y) + 2 * (conf::board_size.x + 2);

}
