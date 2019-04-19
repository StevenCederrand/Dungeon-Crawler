#ifndef _EFFECTS_H
#define _EFFECTS_H
#include <GLM/glm.hpp>
#include <vector>
#include "Graphics/GLinit.h"

class Effects
{
#define MAX_PARTICLES 500

public:
	Effects(GLinit* glInit);
	~Effects();

	void update(float dt);

	void shootEffect(const glm::vec3& pos,const float playerRotation, float speed, float lifetime);
	void hitEffect(const glm::vec3& pos, float lifetime, bool hitEnemy = true);
	const GLuint getVAO() const;
	unsigned int getNrOfAliveParticles() const;

private:
	float size = 0.1f;
	GLfloat m_vertex_buffer_data[12] = {
		-size, -size, 0.0f,
		 size, -size, 0.0f,
		-size,  size, 0.0f,
		 size,  size, 0.0f,
	};
	
	GLfloat m_uv_buffer_data[8] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	struct Particle {
		glm::vec3 velocity;
		glm::vec3 center;
		glm::vec4 color;
		float lifetime;
		float initialLifetime;
		Particle()
			: center(0.0f), velocity(0.0f), color(1.0f), lifetime(0.0f), initialLifetime(0.f) { }

	};
	std::vector<Particle> m_particles;
	unsigned int m_aliveParticles;

	// Used to render it in the renderer class (vao, vbo)
	GLuint m_vao;
	
	GLuint m_verticesVBO;
	GLuint m_uvVBO;
	GLuint m_centerVBO;
	GLuint m_colorVBO;
	void setupGraphicBuffers();

	std::vector<glm::vec3> m_centerPosBuffer;
	std::vector<glm::vec4> m_colorBuffer;
private:
	void updateBuffers();
	size_t m_lastUnusedParticle;
	size_t getFirstUnusedParticle();
};


#endif
