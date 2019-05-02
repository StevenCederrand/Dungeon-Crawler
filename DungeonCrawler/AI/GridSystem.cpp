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
float t = 0.f;

void GridSystem::update(float dt)
{
	GameObject* ptrToLastObject = nullptr;
	int vectorSize = m_occupiedCells.size();

	for (int i = vectorSize - 1; i >= 0; i--)
	{
		GridCell* cell = m_occupiedCells[i];

		int currentCellIndex = getCellIndex(cell->x, cell->z);

		// If game object is in any of the adjacent cells then it's still occupied otherwise we need to free it
		bool freeCell = true;

		GameObject* occupant = cell->info.occupant;

		if (occupant != nullptr) {

			int cellIndexOfGameObject = getCellIndex(occupant->getPosition().x, occupant->getPosition().z);

			// Occupy surrounding cells
			for (int j = 0; j < 9 && freeCell; j++) {

				// Directions [ -1, 0, 1 ]
				int xi = (j % 3) - 1;
				int zi = (j / 3) - 1;

				float newX = cell->x + (xi * m_cellSize);
				float newZ = cell->z + (zi * m_cellSize);

				int newIndex = getCellIndex(newX, newZ);

				if (newIndex != -1 && newIndex == cellIndexOfGameObject)
				{
					freeCell = false;
				}
			}
		}

		if (freeCell)
		{
			cell->setFree();
			m_occupiedCells.erase(m_occupiedCells.begin() + i);
		}

	}

	/*t += dt;
	if (t > 0.5)
	{
		t = 0.f;
		LOG_TRACE("Occupied cells: " + std::to_string(m_occupiedCells.size()));
	}
*/
}

const GridCell& GridSystem::getCell(float x, float z)
{
	int index = getCellIndex(x, z);

	if (index == -1) {
		m_errorGettingGridCell = true;
		return GridCell(0, 0, false);
	}

	m_errorGettingGridCell = false;
	return m_cells[index];
}

const GridCell& GridSystem::getCell(float x, float z, bool occupy, GameObject* occupant)
{
	int index = getCellIndex(x, z);

	if (index == -1) {
		m_errorGettingGridCell = true;
		return GridCell(0, 0, false);
	}

	if (occupy)
	{
		// Occupy surrounding cells
		for (int i = 0; i < 9; i++) {

			// Directions [ -1, 0, 1 ]
			int xi = (i % 3) - 1;
			int zi = (i / 3) - 1;

			float newX = x + (xi * m_cellSize);
			float newZ = z + (zi * m_cellSize);

			int newIndex = getCellIndex(newX, newZ);

			if (newIndex != -1)
			{
			
				if (m_cells[newIndex].info.occupied) continue;

				m_cells[newIndex].setOccupied(occupant);
				m_occupiedCells.emplace_back(&m_cells[newIndex]);
			}

		}
	}

	m_errorGettingGridCell = false;
	return m_cells[index];
}

void GridSystem::occupyCellWithIndex(int index, GameObject* occupant)
{
	GridCell& cell = m_cells[index];

	// Occupy surrounding cells
	for (int i = 0; i < 9; i++) {

		// Directions [ -1, 0, 1 ]
		int xi = (i % 3) - 1;
		int zi = (i / 3) - 1;

		float newX = cell.x + (xi * m_cellSize);
		float newZ = cell.z + (zi * m_cellSize);

		int newIndex = getCellIndex(newX, newZ);

		if (newIndex != -1)
		{

			if (m_cells[newIndex].info.occupied) continue;

			m_cells[newIndex].setOccupied(occupant);
			m_occupiedCells.emplace_back(&m_cells[newIndex]);
		}

	}
}

const int& GridSystem::getCellSize() const
{
	return m_cellSize;
}

const bool GridSystem::failedGettingGridCell() const
{
	return m_errorGettingGridCell;
}

const int GridSystem::getCellIndex(float x, float z)
{
	int cx = (x + m_width * 0.5) / m_cellSize;
	int cz = (z + m_length * 0.5) / m_cellSize;

	int index = cx + cz * (m_width / m_cellSize);

	if (index >= m_cells.size() || index < 0 || cx < 0 || cz < 0) {
		return -1;
	}
	

	return index;

}
