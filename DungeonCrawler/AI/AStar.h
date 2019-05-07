#ifndef _ASTAR_H
#define _ASTAR_H
#include <GameObjects/Room.h>
#include <AI/Node.h>

class AStar {
public:

	AStar();
	~AStar();
	
	std::vector<Node> findPath(GameObject* gameObject, const GridCell& start, const GridCell& destination, Room* room);
	
private:
	float getHCost(const GridCell& start, const GridCell& destination);
	float getHCost(const Node& start, const GridCell& destination);
	bool isInVector(const std::vector<Node*>& nodeVec, const Node& node);
};

#endif