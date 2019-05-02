#ifndef _NODE_H
#define _NODE_H
#include <GLM/glm.hpp>

struct Node
{
	Node(int x, int z, Node* parent, float gCost, float hCost)
	{
		this->x = x;
		this->z = z;
		this->parent = parent;
		this->gCost = gCost;
		this->hCost = hCost;
		this->fCost = this->gCost + this->hCost;
	}

	
	Node* parent = nullptr;
	int x;
	int z;
	float hCost;
	float gCost;
	float fCost;
};

#endif
