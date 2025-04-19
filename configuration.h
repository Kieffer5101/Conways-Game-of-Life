
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
	const sf::Vector2u board_size = {100, 100};
	const unsigned int cell_count = (board_size.x * board_size.y);

}


/*
for (unsigned int i {}; i < conf::cell_count; i++) {
        cellLookup = {1 + (i / conf::board_size.x), 1 + (i % conf::board_size.x)};

        if (i % conf::board_size.x == 0) {
            std::cout << "\n";
            row++;
        }
        if (lifeGrid[cellLookup.x][cellLookup.y] == 0) {
            std::cout << ". ";
            collumn++;
        }
        else {
            std::cout << "# ";
            collumn++;
        }
    }
    std::cout << std::endl;
    std::cout << "Rows : " << row << "\nCollumns : " << collumn/row << std::endl;
*/
