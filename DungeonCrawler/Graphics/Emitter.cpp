#include "Emitter.h"
#include "Utility/Randomizer.h"
#include <algorithm>
#include <Utility/Camera.h>

struct compare {
	// the compiler will automatically inline this
	bool operator()(const Emitter::Particle p1, const Emitter::Particle p2) {
		
		glm::vec3 diff1 = Camera::active->getPosition() - p1.center;
		float l1 = glm::length(diff1);
		
		glm::vec3 diff2 = Camera::active->getPosition() - p2.center;
		float l2 = glm::length(diff2);

		return l1 > l2;
	}
};

Emitter::Emitter(GLinit* glInit, const std::string& texturepath, float sizeOfAnParticle)
{
	m_textureID = glInit->createTexture(texturepath, true, true);

	m_vertex_buffer_data[0] = -sizeOfAnParticle;
	m_vertex_buffer_data[1] = -sizeOfAnParticle;
	m_vertex_buffer_data[2] =  0.0f;

	m_vertex_buffer_data[3] =  sizeOfAnParticle;
	m_vertex_buffer_data[4] = -sizeOfAnParticle;
	m_vertex_buffer_data[5] =  0.0f;

	m_vertex_buffer_data[6] = -sizeOfAnParticle;
	m_vertex_buffer_data[7] =  sizeOfAnParticle;
	m_vertex_buffer_data[8] =  0.0f;

	m_vertex_buffer_data[9] =  sizeOfAnParticle;
	m_vertex_buffer_data[10] = sizeOfAnParticle;
	m_vertex_buffer_data[11] = 0.0f;

	for (size_t i = 0; i < MAX_PARTICLES; i++) {
		m_particles.emplace_back(Particle());
	}

	this->setupGraphicBuffers();

	m_sortTimer = 0.0f;
}

Emitter::~Emitter()
{
	glDeleteBuffers(1, &m_verticesVBO);
	glDeleteBuffers(1, &m_uvVBO);
	glDeleteBuffers(1, &m_centerVBO);
	glDeleteBuffers(1, &m_colorVBO);
	glDeleteVertexArrays(1, &m_vao);
}

void Emitter::update(float dt)
{
	m_centerPosBuffer.clear();
	m_colorBuffer.clear();

	// Update particles
	for (size_t i = 0; i < MAX_PARTICLES; i++)
	{
		Particle& p = m_particles[i];
		// If it's already dead then just move on to the next one
		if (p.lifetime <= 0.0f)
			continue;

		p.lifetime -= dt;

		// So if it was alive and is now dead then decrease the 
		// nr of alive particles
		if (p.lifetime <= 0.0f)
		{
			m_nrOfParticles--;
			continue;
		}

		p.center += p.velocity * dt;
		p.color.a = p.lifetime / p.initialLifetime;

		m_centerPosBuffer.emplace_back(p.center);
		m_colorBuffer.emplace_back(p.color);
	}

	m_sortTimer -= dt;
	if (m_sortTimer <= 0.0f)
	{
		m_sortTimer = 0.20f;
		std::sort(m_particles.begin(), m_particles.end(), compare{});
	}
	
	updateBuffers();

}

void Emitter::addParticle(const glm::vec3& pos, const glm::vec3& velocity, float lifetime, int numberOfParticles)
{

	for (int i = 0; i < numberOfParticles; i++)
	{
		if (m_nrOfParticles == MAX_PARTICLES)
			return;

		size_t pIndex = getFirstUnusedParticle();
		Particle & p = m_particles[pIndex];
		p.center = pos;
		p.velocity = velocity;
		p.lifetime = lifetime;
		p.initialLifetime = lifetime;
		p.color = glm::vec4(1.f);
		m_nrOfParticles++;
	}
}

const GLuint Emitter::getVAO() const
{
	return m_vao;
}

const GLuint Emitter::getTextureID() const
{
	return m_textureID;
}

const unsigned int Emitter::getNumberOfParticles() const
{
	return m_nrOfParticles;
}

void Emitter::setupGraphicBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_verticesVBO);
	glGenBuffers(1, &m_uvVBO);
	glGenBuffers(1, &m_colorVBO);
	glGenBuffers(1, &m_centerVBO);

	glBindVertexArray(m_vao);

	// Buffer for vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex_buffer_data), m_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	// Buffer for uv
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

	// Buffer for color
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(3, 1);


	glBindVertexArray(NULL);
}

void Emitter::updateBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_centerPosBuffer.size() * sizeof(glm::vec3), m_centerPosBuffer.data());
	
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_colorBuffer.size() * sizeof(glm::vec4), m_colorBuffer.data());

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

size_t Emitter::getFirstUnusedParticle()
{
	for (size_t i = m_lastUnusedParticle; i < MAX_PARTICLES; i++)
	{
		if (m_particles[i].lifetime <= 0.0f) {
			m_lastUnusedParticle = i;
			return i;
		}
	}

	for (size_t i = 0; i < MAX_PARTICLES; i++)
	{
		if (m_particles[i].lifetime <= 0.0f) {
			m_lastUnusedParticle = i;
			return i;
		}
	}

	// Else override first one
	m_lastUnusedParticle = 0;
	return 0;

}
