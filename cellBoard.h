
#include <SFML/System.hpp>
#include <vector>

class CellBoard {

private :

	sf::Vector2u board_size;
	int cell_count;
	int generation;
	int neighbors;

	std::vector<int> aliveCells;

	std::vector<std::vector<int>> lifeGrid;
    std::vector<std::vector<int>> lifeGrid_B;

public :

	CellBoard()
		: lifeGrid(50 + 2, std::vector<int>(50 + 2, 0)),
		lifeGrid_B(50 + 2, std::vector<int>(50 + 2, 0)){

		cell_count = (50 * 50);
		this->board_size = board_size;

		generateBoard();
	}

	CellBoard(sf::Vector2u board_size)
		: lifeGrid(board_size.y + 2, std::vector<int>(board_size.x + 2, 0)),
		lifeGrid_B(board_size.y + 2, std::vector<int>(board_size.x + 2, 0)),
		cell_count(board_size.x * board_size.y),
		board_size(board_size),
		generation(0) {

		generateBoard();
	}

	void generateBoard() {

		std::srand(std::time(0));

		for (unsigned int i = 0; i < cell_count; i++) {

			// sets the life or death state of 
			if (std::rand() % 10 + 1 >= 5) {
				birthCell(i);
			}
			else
				killCell(i);
		}
	}

	sf::Vector2u cellLookup(unsigned int index) {

		return {(index / board_size.x) + 1, (index % board_size.x) + 1};
	}

	void updateCells() {

		aliveCells.clear();

		for (unsigned int i = 0; i < cell_count; i++) {

			neighbors +=
				lifeGrid[cellLookup(i).x - 1][cellLookup(i).y - 1]   // top left
				+ lifeGrid[cellLookup(i).x][cellLookup(i).y - 1]     // top center
				+ lifeGrid[cellLookup(i).x + 1][cellLookup(i).y - 1] // top right
				+ lifeGrid[cellLookup(i).x + 1][cellLookup(i).y]     // middle right
				+ lifeGrid[cellLookup(i).x + 1][cellLookup(i).y + 1] // bottom right
				+ lifeGrid[cellLookup(i).x][cellLookup(i).y + 1]     // bottom center
				+ lifeGrid[cellLookup(i).x - 1][cellLookup(i).y + 1] // bottom left
				+ lifeGrid[cellLookup(i).x - 1][cellLookup(i).y];    // middle left
			
			// 
			switch (neighbors) {

			case 1: {
				killCell(i);
				break;
			}
			case 2:
				if (lifeGrid_B[cellLookup(i).x][cellLookup(i).y] == 0)
					break;
				[[fallthrough]];
			case 3: {
				birthCell(i);
				break;
			}
			default:
				killCell(i);
			}
			neighbors = 0;
		}
		lifeGrid = lifeGrid_B;

	}

	/// Births the cell at the specified index within the buffer board
	void birthCell(int index) {

		lifeGrid_B[cellLookup(index).x][cellLookup(index).y] == 1;
		aliveCells.push_back(index);
	}
	void birthCell(int x_pos, int y_pos) {

		lifeGrid_B[x_pos + 1][y_pos + 1] == 1;
		aliveCells.push_back((board_size.x * y_pos) + x_pos);
	}

	/// Kills the cell at the specified index within the buffer board
	void killCell(int index) {

		lifeGrid_B[cellLookup(index).x][cellLookup(index).y] == 0;
	}
	void killCell(int x_pos, int y_pos) {

		lifeGrid_B[x_pos + 1][y_pos + 1] == 0;
	}

};

