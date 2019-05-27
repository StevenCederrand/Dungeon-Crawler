#include "AnimatedEmitter.h"
#include <GLM/gtx/transform.hpp>
#include <GameObjects/GameObject.h>

AnimatedEmitter::AnimatedEmitter(GLinit* glInit, const std::string& spritesheet, unsigned pxWidth, unsigned pxHeight, unsigned frames, float animDelay, bool billboarded, float sizeOfAnParticle)
{
	// Fix animation stuff here
	m_pxWidth = pxWidth;
	m_pxHeight = pxHeight;
	m_frames = frames;
	m_animDelay = animDelay;

	m_textureID = glInit->createTexture(spritesheet, true, true);
	m_billboarded = billboarded;

	m_vertex_buffer_data[0] = -sizeOfAnParticle;
	m_vertex_buffer_data[1] = -sizeOfAnParticle;
	m_vertex_buffer_data[2] = 0.0f;

	m_vertex_buffer_data[3] = sizeOfAnParticle;
	m_vertex_buffer_data[4] = -sizeOfAnParticle;
	m_vertex_buffer_data[5] = 0.0f;

	m_vertex_buffer_data[6] = -sizeOfAnParticle;
	m_vertex_buffer_data[7] = sizeOfAnParticle;
	m_vertex_buffer_data[8] = 0.0f;

	m_vertex_buffer_data[9] = sizeOfAnParticle;
	m_vertex_buffer_data[10] = sizeOfAnParticle;
	m_vertex_buffer_data[11] = 0.0f;

	for (size_t i = 0; i < MAX_PARTICLES; i++) {
		m_particles.emplace_back(Particle());
	}

	this->setupGraphicBuffers();
}

AnimatedEmitter::~AnimatedEmitter()
{
	glDeleteBuffers(1, &m_verticesVBO);
	glDeleteBuffers(1, &m_uvVBO);
	glDeleteBuffers(1, &m_centerVBO);
	glDeleteBuffers(1, &m_colorVBO);
	glDeleteBuffers(1, &m_animationStateVBO);
	glDeleteBuffers(1, &m_modelMatrixVBO);
	glDeleteVertexArrays(1, &m_vao);
}

void AnimatedEmitter::update(float dt)
{
	m_centerPosBuffer.clear();
	m_colorBuffer.clear();
	m_animationIndices.clear();
	m_modelMatrixBuffer.clear();

	// Update particles
	for (size_t i = 0; i < MAX_PARTICLES; i++)
	{
		Particle& p = m_particles[i];
		
		if (!p.immortal) {

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

			p.color.a = p.lifetime / p.initialLifetime;

		}

		p.animTimer += dt;

		if (p.animTimer > m_animDelay)
		{
			p.animState++;
			p.animTimer = 0.0f;
			if (p.animState > m_frames)
				p.animState = 0;

		}

		if (p.parent != nullptr) {
			p.center = p.parent->getPosition();
		}
		
			
		if (p.hadParent && p.parent == nullptr)
		{
			printf("Hello?\n");
			p.hadParent = false;
			p.lifetime = 2.0f;
			p.initialLifetime = 2.0f;
			p.immortal = false;
		}

		

		if (m_billboarded) {
			p.center += p.offset + p.velocity * dt;
		}
		else{
			p.modelMatrix = glm::mat4(1.f);
			p.modelMatrix = glm::translate(p.modelMatrix, p.center + p.offset);
			p.modelMatrix = glm::rotate(p.modelMatrix, glm::radians(p.rotation.x), glm::vec3(1.f, 0.f, 0.f));
			p.modelMatrix = glm::rotate(p.modelMatrix, glm::radians(p.rotation.y), glm::vec3(0.f, 1.f, 0.f));
			p.modelMatrix = glm::rotate(p.modelMatrix, glm::radians(p.rotation.z), glm::vec3(0.f, 0.f, 1.f));
			m_modelMatrixBuffer.emplace_back(p.modelMatrix);
		}

		m_centerPosBuffer.emplace_back(p.center);
		m_colorBuffer.emplace_back(p.color);
		m_animationIndices.emplace_back((float)p.animState);
	}

	updateBuffers();
}

void AnimatedEmitter::addParticle(const glm::vec3& pos, const glm::vec3& velocity, float lifetime, int numberOfParticles)
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
		p.animState = 0;
		p.animTimer = 0.0f;
		p.color = glm::vec4(1.f);
		p.modelMatrix = glm::mat4(1.0f);
		m_nrOfParticles++;
	}
}

void AnimatedEmitter::addParticle(const glm::vec3& pos, const glm::vec3& rotation, const glm::vec3& velocity, float lifetime, int numberOfParticles)
{
	for (int i = 0; i < numberOfParticles; i++)
	{
		if (m_nrOfParticles == MAX_PARTICLES)
			return;

		size_t pIndex = getFirstUnusedParticle();
		Particle & p = m_particles[pIndex];
		p.center = pos;
		p.velocity = velocity;
		p.rotation = rotation;
		p.lifetime = lifetime;
		p.animState = 0;
		p.animTimer = 0.0f;
		p.initialLifetime = lifetime;
		p.modelMatrix = glm::mat4(1.0f);
		p.color = glm::vec4(1.f);

		m_nrOfParticles++;
	}
}

void AnimatedEmitter::addParticle(GameObject* parent, const glm::vec3& offset, int numberOfParticles)
{
	for (int i = 0; i < numberOfParticles; i++)
	{
		if (m_nrOfParticles == MAX_PARTICLES)
			return;

		size_t pIndex = getFirstUnusedParticle();
		Particle & p = m_particles[pIndex];
		p.parent = parent;
		p.center = parent->getPosition();
		p.offset = offset;
		p.animState = 0;
		p.animTimer = 0.0f;
		p.immortal = true;
		p.hadParent = true;
		p.color = glm::vec4(1.f);
		p.modelMatrix = glm::mat4(1.0f);
		m_nrOfParticles++;
	}
}

void AnimatedEmitter::addParticle(GameObject* parent, const glm::vec3& offset, const glm::vec3& rotation, int numberOfParticles)
{
	for (int i = 0; i < numberOfParticles; i++)
	{
		if (m_nrOfParticles == MAX_PARTICLES)
			return;

		size_t pIndex = getFirstUnusedParticle();
		Particle & p = m_particles[pIndex];
		p.parent = parent;
		p.center = parent->getPosition();
		p.offset = offset;
		p.rotation = rotation;
		p.animState = 0;
		p.animTimer = 0.0f;
		p.immortal = true;
		p.hadParent = true;
		p.color = glm::vec4(1.f);

		m_nrOfParticles++;
	}
}

const GLuint AnimatedEmitter::getVAO() const
{
	return m_vao;
}

const GLuint AnimatedEmitter::getTextureID() const
{
	return m_textureID;
}

const unsigned int AnimatedEmitter::getNumberOfParticles() const
{
	return m_nrOfParticles;
}

const unsigned int AnimatedEmitter::getNumberOfAnimationFrames() const
{
	return m_frames;
}

const bool AnimatedEmitter::isBillBoarded() const
{
	return m_billboarded;
}

void AnimatedEmitter::setupGraphicBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Buffer for vertices
	glGenBuffers(1, &m_verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex_buffer_data), m_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	// Buffer for uv
	glGenBuffers(1, &m_uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_uv_buffer_data), m_uv_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	// Buffer for centerPositions
	glGenBuffers(1, &m_centerVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(2, 1);

	// Buffer for color
	glGenBuffers(1, &m_colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(3, 1);

	// Animation index
	glGenBuffers(1, &m_animationStateVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_animationStateVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(4, 1);


	if (!m_billboarded) {

		glGenBuffers(1, &m_modelMatrixVBO);
		
		// Buffer for model matrices
		glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixVBO);
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec4) * 4, NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(0 * sizeof(glm::vec4)));
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(1 * sizeof(glm::vec4)));
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(2 * sizeof(glm::vec4)));
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(3 * sizeof(glm::vec4)));
		glBindBuffer(GL_ARRAY_BUFFER, NULL);


		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
	}

	glBindVertexArray(NULL);

}

void AnimatedEmitter::updateBuffers()
{

	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_centerPosBuffer.size() * sizeof(glm::vec3), m_centerPosBuffer.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_colorBuffer.size() * sizeof(glm::vec4), m_colorBuffer.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_animationStateVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_animationIndices.size() * sizeof(float), m_animationIndices.data());

	if (!m_billboarded) {
		glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixVBO);
		glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec4) * 4, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_modelMatrixBuffer.size() * sizeof(glm::vec4) * 4, m_modelMatrixBuffer.data());
	}

	glBindBuffer(GL_ARRAY_BUFFER, NULL);

}

size_t AnimatedEmitter::getFirstUnusedParticle()
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
