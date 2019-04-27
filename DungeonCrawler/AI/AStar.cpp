#include "AStar.h"
#include <algorithm>

bool nodeComparator(const Node* n1, const Node* n2)
{
	if (n2->fCost > n1->fCost) 
		return true;
	else
		return false;
}

AStar::AStar()
{
}

AStar::~AStar()
{
}

float AStar::getHCost(const GridCell& start, const GridCell& destination)
{
	float diffX = start.x - destination.x;
	float diffZ = start.z - destination.z;

	return sqrtf(diffX * diffX + diffZ * diffZ);

}

float AStar::getHCost(const Node& start, const GridCell& destination)
{
	float diffX = start.x - destination.x;
	float diffZ = start.z - destination.z;

	return sqrtf(diffX * diffX + diffZ * diffZ);

}

bool AStar::isInVector(const std::vector<Node*>& nodeVec, const Node& node)
{
	for (const Node* n : nodeVec)
	{
		if (n->x == node.x && n->z == node.z)
			return true;
	}

	return false;
}

std::vector<Node> AStar::findPath(const GridCell& start, const GridCell& destination, Room* room)
{
	std::vector<Node*> openList;
	openList.reserve(250);
	std::vector<Node*> closedList;
	closedList.reserve(250);
	
	Node* current = new Node(start.x, start.z, nullptr, 0, getHCost(start, destination));
	openList.emplace_back(current);
	while (openList.size() > 0){
		
		std::stable_sort(openList.begin(), openList.end(), nodeComparator);
		current = openList[0];
		Node* pNode = openList[0];

		if (current->x == destination.x && current->z == destination.z) {

			std::vector<Node> path;
			path.reserve(openList.size());
			
			while (current->parent != nullptr)
			{
				path.emplace_back(*current);
				current = current->parent;
			}

			for (int i = 0; i < openList.size(); i++)
			{
				if(openList[i])
					delete openList[i];
			}

			for (int i = 0; i < closedList.size(); i++)
			{
				if (closedList[i])
					delete closedList[i];
			}

			openList.clear();
			closedList.clear();
			return path;

		}
		
		openList.erase(openList.begin());
		closedList.emplace_back(current);

		// Look at the adjacent cells
		for (int i = 0; i < 9; i++) {
			if (i == 4) continue;

			// Directions [ -1, 0, 1 ]
			int xi = (i % 3) - 1;
			int zi = (i / 3) - 1;

			const GridCell& c = room->getGrid()->getCell(current->x + (xi * (room->getGrid()->getCellSize())), current->z + (zi * (room->getGrid()->getCellSize())));

			if (!c.valid) continue;

			float gCost = current->gCost + getHCost(*current, c);
			float hCost = getHCost(c, destination);

			Node* node = new Node(c.x, c.z, pNode, gCost, hCost);

			if (isInVector(closedList, *node) && gCost >= current->gCost) {
				delete node;
				continue;
			}

			if (!isInVector(openList, *node) || gCost < current->gCost)
			{
				openList.emplace_back(node);
			}
			else
			{
				delete node;
			}
		}

	}

	for (int i = 0; i < closedList.size(); i++)
	{
		if (closedList[i])
			delete closedList[i];
	}
	closedList.clear();
	
	// Default return
	return std::vector<Node>();
}
