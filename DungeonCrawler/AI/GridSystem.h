#ifndef _GRIDSYSTEM_H
#define _GRIDSYSTEM_H
#include <GLM/glm.hpp>
#include "GridCell.h"
#include <vector>

class Room;

class GridSystem
{
public:
	GridSystem(int width, int length, int cellSize, Room* room);
	~GridSystem();

	void constructCells();
	void update(float dt);

	const GridCell& getCell(float x, float z);
	const int& getCellSize() const;
	const bool failedGettingGridCell() const;

private:
	std::vector<GridCell> m_cells;
	Room* m_room;
	
	int m_width;
	int m_length;
	int m_cellSize;
	bool m_errorGettingGridCell;

};


#endif
