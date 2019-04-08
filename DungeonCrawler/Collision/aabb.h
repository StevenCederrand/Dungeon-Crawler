#ifndef _AABB_H
#define _AABB_H
#include <GLM/glm.hpp>

class AABB {
public:
	AABB(const glm::vec3& min, const glm::vec3& max);
	~AABB();

	void setPosition(const glm::vec3& position);
	bool checkCollision(const AABB& other);

	const glm::vec3& getPosition() const;
	const glm::vec3& getDimensions() const;

private:
	glm::vec3 m_position;
	glm::vec3 m_dimensions;
};


#endif
