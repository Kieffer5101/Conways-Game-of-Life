
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>

class CellBoard {

private :

	sf::Vector2i board_size;
	int cell_count;
	int generation;
	int population;
	int neighbors;
	int before;
	int current;

	std::vector<int> aliveCells;
	std::vector<int> updatedCells;

	std::vector<std::vector<int>> lifeGrid;   // The grid of cells
    std::vector<std::vector<int>> lifeGrid_B; // The buffer grid

public :

	CellBoard()
		: lifeGrid(50 + 2, std::vector<int>(50 + 2, 0)),
		lifeGrid_B(50 + 2, std::vector<int>(50 + 2, 0)){

		cell_count = (50 * 50);
		this->board_size = board_size;

		generateBoard();
	}

	CellBoard(sf::Vector2i board_size)
		: lifeGrid(board_size.y + 2, std::vector<int>(board_size.x + 2, 0)),
		lifeGrid_B(board_size.y + 2, std::vector<int>(board_size.x + 2, 0)),
		cell_count(board_size.x * board_size.y),
		board_size(board_size),
		generation(0),
		before(0),
		current(0){

		generateBoard();
	}

	void generateBoard() {

		std::srand(std::time(0));

		for (unsigned int i = 0; i < cell_count; i++) {

			// sets the life or death state of
			bool random = std::rand() % 2 == 1; // outside of if statement for debugging
			if (random) {
				birthCell(i);
			}
			else
				killCell(i);
		}
		lifeGrid = lifeGrid_B;
	}
	void clearBoard() {
		for (unsigned int i = 0; i < cell_count; i++) {

			killCell(i);
		}
		lifeGrid = lifeGrid_B;
	}

	std::vector<std::vector<int>> getBoard() {
		return lifeGrid_B;
	}
	
	std::vector<int> getUpdatedCells() {
		return updatedCells;
	}

	std::vector<int> getAliveCells() {
		return aliveCells;
	}

	int getGeneration() {
		return generation;
	}

	int getPopulation() {
		return population;
	}
		
	sf::Vector2u cellLookup(unsigned int index) {
		return {(index / board_size.x) + 1, (index % board_size.x) + 1};
	}

	void updateCells() {

		aliveCells.clear();
		population = 0;

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
			
			before = lifeGrid[cellLookup(i).x][cellLookup(i).y];
			
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

			current = lifeGrid_B[cellLookup(i).x][cellLookup(i).y];
			if (current != before || lifeGrid_B[cellLookup(i).x][cellLookup(i).y] == 1) // check if the cell has been updated and is alive
				updatedCells.push_back(i);

		}
		generation++;
		useBuffer();
		
		}

	void clearUpdatedCells() {
		updatedCells.clear();
	}

	/// Births the cell at the specified index within the buffer board
	void birthCell(int index) {

		lifeGrid_B[cellLookup(index).x][cellLookup(index).y] = 1;
		aliveCells.push_back(index);
		
		population++;
	}
	void birthCell(int x_pos, int y_pos) {

		lifeGrid_B[x_pos + 1][y_pos + 1] = 1;
		aliveCells.push_back((board_size.x * y_pos) + x_pos);
		population++;
	}

	/// Kills the cell at the specified index within the buffer board
	void killCell(int index) {

		lifeGrid_B[cellLookup(index).x][cellLookup(index).y] = 0;
	}
	void killCell(int x_pos, int y_pos) {

		lifeGrid_B[x_pos + 1][y_pos + 1] = 0;
	}

	void useBuffer() {
		lifeGrid = lifeGrid_B;
	}

};