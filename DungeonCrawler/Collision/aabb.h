#ifndef _AABB_H
#define _AABB_H
#include <GLM/glm.hpp>
#include <vector>
#include <GLM/glm.hpp>

class AABB {

public:
	AABB(const glm::vec3& min, const glm::vec3& max);
	AABB();
	~AABB();

	void setParentPosition(const glm::vec3& position);
	void setDimensions(const float& width, const float& height, const float& depth);
	
	float swepAABB(const glm::vec3& vel, const AABB& other, float& normalX, float& normalZ);
	bool checkCollision(const AABB& other);
	bool checkCollisionNode(glm::vec3 min, glm::vec3 max);
	bool checkCollisionWithRay(const glm::vec3& rayOrigin, const glm::vec3 rayDirection, float& t);
	bool checkCollisionWithCell(const glm::vec2& cellPosition, float size);

	const glm::vec3& getPosition() const;
	const glm::vec3& getDimensions() const;

private:
	glm::vec3 m_parentPosition;
	glm::vec3 m_position;
	glm::vec3 m_dimensions;

};


#endif
