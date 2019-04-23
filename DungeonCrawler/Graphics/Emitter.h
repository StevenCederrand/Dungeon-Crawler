#ifndef _EMITTER_H
#define _EMITTER_H
#include <GLM/glm.hpp>
#include <vector>
#include "Graphics/GLinit.h"

#define MAX_PARTICLES 120

class Emitter {
public:
	Emitter(GLinit* glInit, const std::string& texturepath, float sizeOfAnParticle);
	~Emitter();

	void update(float dt);
	void addParticle(const glm::vec3& pos, float speed, float lifetime, int numberOfParticles = 1);
	const GLuint getVAO() const;
	const GLuint getTextureID() const;
	const unsigned int getNumberOfParticles() const;

private:
	struct Particle {
		glm::vec3 velocity;
		glm::vec3 center;
		glm::vec4 color;
		float lifetime;
		float initialLifetime;
		Particle()
			: center(0.0f), velocity(0.0f), color(1.0f), lifetime(0.0f), initialLifetime(0.f) { }

	};

	GLuint m_textureID = -1;
	unsigned int m_nrOfParticles = 0;
	unsigned int m_lastUnusedParticle = 0;

	std::vector<Particle> m_particles;
	std::vector<glm::vec3> m_centerPosBuffer;
	std::vector<glm::vec4> m_colorBuffer;

	GLuint m_vao;
	GLuint m_verticesVBO;
	GLuint m_uvVBO;
	GLuint m_centerVBO;
	GLuint m_colorVBO;

	GLfloat m_vertex_buffer_data[12];
	GLfloat m_uv_buffer_data[8] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

private:
	void setupGraphicBuffers();
	void updateBuffers();
	size_t getFirstUnusedParticle();
};


#endif