#include "Effects.h"
#include "System/LOG.h"
#include <GLM/gtx/norm.hpp>
#include "Globals/Paths.h"
#include "Utility/Randomizer.h"
#include "Utility/Camera.h"
#include <algorithm>

bool compareParticles(Effects::Particle& p1, Effects::Particle& p2) {

	float len_p1 = glm::length2(p1.center - Camera::active->getPosition());
	float len_p2 = glm::length2(p2.center - Camera::active->getPosition());

	return len_p1 < len_p2;

}

/*
	FIX SORTING BRO
*/

Effects::Effects(GLinit* glInit)
{
	this->setupGraphicBuffers();
	
	// Setup types
	m_bloodType.textureID = glInit->createTexture("BloodParticle.png", true, true);

	// Add the types
	m_types.emplace_back(&m_bloodType);

	// Fill the arrays
	for (size_t i = 0; i < MAX_PARTICLES; i++){
		m_bloodParticles.emplace_back(Particle());
	}
	
}

Effects::~Effects()
{
	glDeleteBuffers(1, &m_verticesVBO);
	glDeleteBuffers(1, &m_uvVBO);
	glDeleteBuffers(1, &m_centerVBO);
	glDeleteBuffers(1, &m_colorVBO);
	glDeleteVertexArrays(1, &m_vao);
}

void Effects::update(float dt)
{
	m_centerPosBuffer.clear();
	m_colorBuffer.clear();
	
	// Update blood particles
	for (size_t i = 0; i < MAX_PARTICLES; i++)
	{
		Particle& p = m_bloodParticles[i];
		// If it's already dead then just move on to the next one
		if (p.lifetime <= 0.0f)
			continue;

		p.lifetime -= dt;

		// So if it was alive and is now dead then decrease the 
		// nr of alive particles
		if (p.lifetime <= 0.0f)
		{
			m_bloodType.nrOfParticles--;
			continue;
		}

		p.center += p.velocity * dt;
		p.color.a = p.lifetime / p.initialLifetime;

		m_centerPosBuffer.emplace_back(p.center);
		m_colorBuffer.emplace_back(p.color);

	}


	updateBuffers();
	
}

void Effects::shootEffect(const glm::vec3& pos, const float playerRotation, float speed, float lifetime)
{
	
	float xMul = cosf(glm::radians(playerRotation));
	float zMul = sinf(glm::radians(playerRotation));
	
	for(int i = 0; i < 5; i++)
	{
		if (m_bloodType.nrOfParticles == MAX_PARTICLES)
			return;
		
		size_t pIndex = getFirstUnusedParticle(m_bloodType, m_bloodParticles);
		Particle& p = m_bloodParticles[pIndex];
		p.center = pos + glm::vec3(xMul + Randomizer::single(-1.f, 1.f) / 6.f, 0.5f, zMul + Randomizer::single(-1.f, 1.f) / 6.f);
		p.velocity = glm::vec3(0.f, Randomizer::single(speed / 2.f, speed), 0.f);
		p.lifetime = lifetime;
		p.initialLifetime = lifetime;
		p.color = glm::vec4(1.f);
		m_bloodType.nrOfParticles++;
	}
	
}

void Effects::hitEffect(const glm::vec3& pos, float lifetime, bool hitEnemy)
{
	float offset = 100.f;
	int nrOfParticels;
	glm::vec4 particleColor;

	if (hitEnemy) {
		nrOfParticels = 5;
		particleColor = glm::vec4(1.0f, 0.1f, 0.1f, 1.0f);
	}
	else {
		nrOfParticels = 5;
		particleColor = glm::vec4(0.f, 0.0f, 0.0f, 1.0f);
	}

	for (int i = 0; i < nrOfParticels; i++)
	{
		if (m_bloodType.nrOfParticles == MAX_PARTICLES)
			return;

		size_t p2Index = getFirstUnusedParticle(m_bloodType, m_bloodParticles);
		Particle& p2 = m_bloodParticles[p2Index];
		p2.center = pos - Randomizer::vec3(-offset, offset, 0.f, 0.f, -offset, offset) / (offset * 1.5f);
		p2.center.y = 0.5f;
		p2.velocity = glm::vec3(0.f, Randomizer::single(-2.f, 7.f), 0.f);
		p2.lifetime = 0.2f;
		p2.initialLifetime = p2.lifetime;
		p2.color = particleColor;

		m_bloodType.nrOfParticles++;
	}
}

const GLuint Effects::getVAO() const
{
	return m_vao;
}

const std::vector<Effects::ParticleType*>& Effects::getTypeVector() const
{
	return m_types;
}

const unsigned int Effects::getNumberOfParticles() const
{
	unsigned int temp = 0;
	for (size_t i = 0; i < m_types.size(); i++)
	{
		temp += m_types[i]->nrOfParticles;
	}

	return temp;
}

void Effects::setupGraphicBuffers()
{
	
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_verticesVBO);
	glGenBuffers(1, &m_uvVBO);
	glGenBuffers(1, &m_centerVBO);
	glGenBuffers(1, &m_colorVBO);

	glBindVertexArray(m_vao);
	
	// Buffer for vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex_buffer_data), m_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	// Buffer for vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_uv_buffer_data), m_uv_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	// Buffer for centerPositions
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(2, 1);

	// Buffer for the lengths and scale
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(NULL);

}
void Effects::updateBuffers(){
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_centerPosBuffer.size() * sizeof(glm::vec3), m_centerPosBuffer.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_colorBuffer.size() * sizeof(glm::vec4), m_colorBuffer.data());

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

size_t Effects::getFirstUnusedParticle(ParticleType& type, const std::vector<Particle>& vec)
{
	for (size_t i = type.lastUnusedParticle; i < MAX_PARTICLES; i++)
	{
		if (vec[i].lifetime <= 0.0f) {
			type.lastUnusedParticle = i;
			return i;
		}
	}

	for (size_t i = 0; i < MAX_PARTICLES; i++)
	{
		if (vec[i].lifetime <= 0.0f) {
			type.lastUnusedParticle = i;
			return i;
		}
	}

	// Else override first one
	type.lastUnusedParticle = 0;
	return 0;

}
