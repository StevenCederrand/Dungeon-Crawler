#ifndef _AABB_H
#define _AABB_H
#include <GLM/glm.hpp>
class AABB {
public:
	AABB(glm::vec3 minX, glm::vec3 minY, glm::vec3 minZ, glm::vec3 maxX, glm::vec3 maxY, glm::vec3 maxZ)
	{
		this->minX = minX;
		this->minY = minY;
		this->minZ = minZ;
		this->maxX = maxX;
		this->maxY = maxY;
		this->maxZ = maxZ;
	}

	bool isColliding(const AABB& other)
	{

		return false;
	}


private:
	glm::vec3 minX, minY, minZ, maxX, maxY, maxZ;



};


#endif
