#include "GridSystem.h"
#include <System/Log.h>
#include <GameObjects/Room.h>

GridSystem::GridSystem(int width, int length, int cellSize, Room* room)
{
	m_width = width;
	m_length = length;
	m_cellSize = cellSize;
	m_room = room;
}

GridSystem::~GridSystem()
{
}

void GridSystem::constructCells()
{
	int x_nrOfCells = m_width / m_cellSize;
	int z_nrOfCells = m_length / m_cellSize;
	const std::vector<AABB*>& boundingBoxes = m_room->getBoundingBoxes();
	const glm::vec2 centerPos = glm::vec2(m_room->getPosition().x, m_room->getPosition().z);

	for (int z = 0; z < z_nrOfCells; z++)
	{
		for (int x = 0; x < x_nrOfCells; x++)
		{
			glm::vec2 cellPos = centerPos + glm::vec2((x * m_cellSize) - m_width * 0.5, (z * m_cellSize) - m_length * 0.5);
			bool valid = true;
			// Check collision
			for (int i = 0; i < boundingBoxes.size() && valid; i++)
			{
				AABB* bb = boundingBoxes[i];
				valid = bb->checkCollisionWithCell(cellPos, m_cellSize * 0.5f);
			}
			
			m_cells.emplace_back(GridCell(cellPos.x, cellPos.y, valid));
		}
	}

}

void GridSystem::update(float dt)
{
	
}

const GridCell& GridSystem::getCell(float x, float z)
{
	int cx = (x + m_width * 0.5) / m_cellSize;
	int cz = (z + m_length * 0.5) / m_cellSize;

	int index = cx + cz * (m_width / m_cellSize);
	
	if (index >= m_cells.size() || index < 0 || cx < 0 || cz < 0) {
		m_errorGettingGridCell = true;
		return GridCell(0, 0, false);

	}
	m_errorGettingGridCell = false;
	return m_cells[index];
}

const int& GridSystem::getCellSize() const
{
	return m_cellSize;
}

const bool GridSystem::failedGettingGridCell() const
{
	return m_errorGettingGridCell;
}
