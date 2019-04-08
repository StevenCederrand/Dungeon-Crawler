#include "AABB.h"

AABB::AABB(const glm::vec3 & min, const glm::vec3 & max)
{
	m_dimensions = (max - min) * 0.5f;
}

AABB::~AABB()
{
}

void AABB::setPosition(const glm::vec3 & position)
{
	m_position = position;
}

bool AABB::checkCollision(const AABB & other)
{

	// Check x and z for now
	if (m_position.x + m_dimensions.x > other.m_position.x - other.m_dimensions.x && 
		m_position.x - m_dimensions.x < other.m_position.x + other.m_dimensions.x &&
		m_position.z + m_dimensions.z > other.m_position.z - other.m_dimensions.z && 
		m_position.z - m_dimensions.z < other.m_position.z + other.m_dimensions.z)
	{
		return true;
	}


	return false;
}

const glm::vec3 & AABB::getPosition() const
{
	return m_position;
}

const glm::vec3 & AABB::getDimensions() const
{
	return m_dimensions;
}
