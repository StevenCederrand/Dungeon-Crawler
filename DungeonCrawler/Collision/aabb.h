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


	const glm::vec3& getPosition() const;
	const glm::vec3& getDimensions() const;

private:
	glm::vec3 m_parentPosition;
	glm::vec3 m_position;
	glm::vec3 m_dimensions;

};


#endif
