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
		xInvEntry = (other.getPosition().x - other.m_dimensions.x) - (m_parentPosition.x + m_dimensions.x);
		xInvExit = (other.getPosition().x + other.m_dimensions.x) - (m_parentPosition.x - m_dimensions.x);
	}
	else
	{
		xInvEntry = (other.getPosition().x + other.m_dimensions.x) - (m_parentPosition.x - m_dimensions.x);
		xInvExit = (other.getPosition().x - other.m_dimensions.x) - (m_parentPosition.x + m_dimensions.x);
	}

	// Z-AXIS
	if (vel.z > 0.0f)
	{
		zInvEntry = (other.getPosition().z - other.m_dimensions.z) - (m_parentPosition.z + m_dimensions.z);
		zInvExit = (other.getPosition().z + other.m_dimensions.z) - (m_parentPosition.z - m_dimensions.z);
	}
	else
	{
		zInvEntry = (other.getPosition().z + other.m_dimensions.z) - (m_parentPosition.z - m_dimensions.z);
		zInvExit = (other.getPosition().z - other.m_dimensions.z) - (m_parentPosition.z + m_dimensions.z);
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
		if (m_parentPosition.x + m_dimensions.x < other.getPosition().x - other.m_dimensions.x ||
			m_parentPosition.x - m_dimensions.x > other.getPosition().x + other.m_dimensions.x)
			return 1.0f;
	}

	if (zEntry < 0.0f)
	{
		if (m_parentPosition.z + m_dimensions.z < other.getPosition().z - other.m_dimensions.z ||
			m_parentPosition.z - m_dimensions.z > other.getPosition().z + other.m_dimensions.z)
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
		!(m_parentPosition.x + m_dimensions.x < other.getPosition().x - other.m_dimensions.x ||
		m_parentPosition.x - m_dimensions.x >  other.getPosition().x + other.m_dimensions.x ||
		m_parentPosition.z + m_dimensions.z < other.getPosition().z - other.m_dimensions.z ||
		m_parentPosition.z - m_dimensions.z > other.getPosition().z + other.m_dimensions.z);
}

bool AABB::checkCollisionWithRay(const glm::vec3 & rayOrigin, const glm::vec3 rayDirection, float & t)
{
	float t1 = 1000, t0 = -t1;

	for (int i = 0; i < 3; i++)
	{
		if (rayDirection[i] != 0.0f)
		{
			float lo = (m_parentPosition[i] + m_position[i] - m_dimensions[i] - rayOrigin[i]) / rayDirection[i];
			float hi = (m_parentPosition[i] + m_position[i] + m_dimensions[i] - rayOrigin[i]) / rayDirection[i];

			t0 = std::max(t0, std::min(lo, hi));
			t1 = std::min(t1, std::max(lo, hi));
		}
		else
		{
			if (rayOrigin[i] < m_parentPosition[i] + m_position[i] - m_dimensions[i] ||
				rayOrigin[i] > m_parentPosition[i] + m_position[i] + m_dimensions[i])
			{
				return false;
			}
		}
	}

	t = t0;

	return (t0 <= t1) && (t1 > 0);
}

const glm::vec3 & AABB::getPosition() const
{
	return m_parentPosition + m_position;
}

const glm::vec3 & AABB::getDimensions() const
{
	return m_dimensions;
}

