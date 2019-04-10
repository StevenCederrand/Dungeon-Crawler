#include "AABB.h"
#include <limits>
#include <algorithm>
#include <System/Log.h>

AABB::AABB(const glm::vec3 & min, const glm::vec3 & max)
{
	m_dimensions = (max - min) * 0.5f;
	m_position = max - (max - min) * 0.5f;

}

AABB::AABB()
{
}

AABB::~AABB()
{
}

void AABB::setParentPosition(const glm::vec3 & position)
{
	m_parentPosition = position;
}

void AABB::setDimensions(const float & width, const float & height, const float & depth)
{
	m_dimensions.x = width;
	m_dimensions.y = height;
	m_dimensions.z = depth;
}

float AABB::swepAABB(const glm::vec3 & vel, const AABB & other, float & normalX, float & normalZ)
{

	float xInvEntry, zInvEntry;
	float xInvExit, zInvExit;

	// X-AXIS
	if (vel.x > 0.0f)
	{
		xInvEntry = (other.m_parentPosition.x - other.m_dimensions.x) - (m_parentPosition.x + m_dimensions.x);
		xInvExit = (other.m_parentPosition.x + other.m_dimensions.x) - (m_parentPosition.x - m_dimensions.x);
	}
	else
	{
		xInvEntry = (other.m_parentPosition.x + other.m_dimensions.x) - (m_parentPosition.x - m_dimensions.x);
		xInvExit = (other.m_parentPosition.x - other.m_dimensions.x) - (m_parentPosition.x + m_dimensions.x);
	}

	// Z-AXIS
	if (vel.z > 0.0f)
	{
		zInvEntry = (other.m_parentPosition.z - other.m_dimensions.z) - (m_parentPosition.z + m_dimensions.z);
		zInvExit = (other.m_parentPosition.z + other.m_dimensions.z) - (m_parentPosition.z - m_dimensions.z);
	}
	else
	{
		zInvEntry = (other.m_parentPosition.z + other.m_dimensions.z) - (m_parentPosition.z - m_dimensions.z);
		zInvExit = (other.m_parentPosition.z - other.m_dimensions.z) - (m_parentPosition.z + m_dimensions.z);
	}


	// Find time of collision
	float xEntry, zEntry;
	float xExit, zExit;

	if (vel.x == 0.0f) 
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / vel.x;
		xExit = xInvExit / vel.x;
	}

	if (vel.z == 0.0f)
	{
		zEntry = -std::numeric_limits<float>::infinity();
		zExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		zEntry = zInvEntry / vel.z;
		zExit = zInvExit / vel.z;
	}

	// Early and latest time
	float entryTime = std::max(xEntry, zEntry);
	float exitTime = std::min(xExit, zExit);

	if (entryTime > exitTime) return 1.0f;
	if (xEntry < 0.0f && zEntry < 0.0f) return 1.0f;
	
	if (xEntry < 0.0f)
	{
		if (m_parentPosition.x + m_dimensions.x < other.m_parentPosition.x - other.m_dimensions.x ||
			m_parentPosition.x - m_dimensions.x > other.m_parentPosition.x + other.m_dimensions.x)
			return 1.0f;
	}

	if (zEntry < 0.0f)
	{
		if (m_parentPosition.z + m_dimensions.z < other.m_parentPosition.z - other.m_dimensions.z ||
			m_parentPosition.z - m_dimensions.z > other.m_parentPosition.z + other.m_dimensions.z)
			return 1.0f;
	}

	if (xEntry > zEntry)
	{
		
		if (xInvEntry < 0.0f)
		{
			normalX = 1.0f;
			normalZ = 0.0f;
		}
		else
		{
			normalX = -1.0f;
			normalZ = 0.0f;
		}
	
	}
	else
	{
		if (zInvEntry < 0.0f)
		{
			normalX = 0.0f;
			normalZ = 1.0f;
		}
		else
		{
			normalX = 0.0f;
			normalZ = -1.0f;
		}
		
	}

	return entryTime;
}

bool AABB::checkCollision(const AABB & other)
{
	return 
		!(m_parentPosition.x + m_dimensions.x < other.m_parentPosition.x - other.m_dimensions.x || 
		m_parentPosition.x - m_dimensions.x >  other.m_parentPosition.x + other.m_dimensions.x || 
		m_parentPosition.z + m_dimensions.z < other.m_parentPosition.z - other.m_dimensions.z ||
		m_parentPosition.z - m_dimensions.z > other.m_parentPosition.z + other.m_dimensions.z);
}

const glm::vec3 & AABB::getPosition() const
{
	return m_parentPosition + m_position;
}

const glm::vec3 & AABB::getDimensions() const
{
	return m_dimensions;
}

