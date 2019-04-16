#include "Effects.h"
#include "System/LOG.h"
#include <math.h>

Effects::Effects()
{
	this->setupGraphicBuffers();

	for (int i = 0; i < MAX_LASERS; i++)
	{
		LaserStruct& laser = m_lasers[i];
		laser.currentLifetime = 0.0f;
	}
}

Effects::~Effects()
{
	glDeleteBuffers(1, &m_verticesVBO);
	glDeleteBuffers(1, &m_centerVBO);
	glDeleteBuffers(1, &m_lengthsAndScaleVBO);
	glDeleteVertexArrays(1, &m_vao);
}

void Effects::update(float dt)
{
	m_positionBuffer.clear();
	m_lengthsAndScaleBuffer.clear();

	for (int i = 0; i < m_nrAliveLasers; i++)
	{
		LaserStruct& laser = m_lasers[i];

		//laser.currentLifetime -= dt;

		if (laser.currentLifetime <= 0.0f)
		{
			if (i != m_nrAliveLasers - 1)
				swap(laser, m_lasers[m_nrAliveLasers - 1]);
			m_nrAliveLasers--;
			continue;
		}

		m_positionBuffer.push_back(laser.centerpos);
		m_lengthsAndScaleBuffer.push_back(laser.lengthsAndScales);

	}

	updateBuffers();

}

void Effects::addLaser(const glm::vec3& startpos, const glm::vec3& endpos, float lifetime)
{
	float xLength = endpos.x - startpos.x;
	float zLength = endpos.z - startpos.z;

	float angle = atan2f(zLength, xLength);
	float xMul = cosf(angle);
	float zMul = sinf(angle);

	if (m_nrAliveLasers < MAX_LASERS)
	{
		LaserStruct& laser = m_lasers[m_nrAliveLasers];
		
		// Center between the two positions 
		laser.centerpos.x = startpos.x + (xLength * 0.5) * xMul;
		laser.centerpos.y = startpos.y + 0.5f;
		laser.centerpos.z = startpos.z + (zLength * 0.5) * zMul;

		// Lengths and scales
		laser.lengthsAndScales.x = xLength;
		laser.lengthsAndScales.y = zLength;
		laser.lengthsAndScales.z = abs(xMul);
		laser.lengthsAndScales.w = abs(zMul);

		laser.initialLifetime = lifetime;
		laser.currentLifetime = lifetime;

		m_nrAliveLasers++;
	}

}

const GLuint Effects::getVAO() const
{
	return m_vao;
}

unsigned int Effects::getNrOfAliveLasers() const
{
	return m_nrAliveLasers;
}

void Effects::setupGraphicBuffers()
{
	
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_verticesVBO);
	glGenBuffers(1, &m_centerVBO);
	glGenBuffers(1, &m_lengthsAndScaleVBO);

	glBindVertexArray(m_vao);
	
	// Buffer for vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex_buffer_data), m_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	// Buffer for centerPositions
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_LASERS * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(1, 1);

	// Buffer for the lengths and scale
	glBindBuffer(GL_ARRAY_BUFFER, m_lengthsAndScaleVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_LASERS * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(NULL);

}
void Effects::updateBuffers(){
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_LASERS * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_positionBuffer.size() * sizeof(glm::vec3), m_positionBuffer.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_lengthsAndScaleVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_LASERS * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_lengthsAndScaleBuffer.size() * sizeof(glm::vec4), m_lengthsAndScaleBuffer.data());
}

void Effects::swap(LaserStruct& l1, LaserStruct& l2){
	
	LaserStruct temp = l1;
	l1 = l2;
	l2 = temp;
}