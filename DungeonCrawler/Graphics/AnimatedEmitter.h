#ifndef _ANIMATEDEMITTER_H
#define _ANIMATEDEMITTER_H

#include<GLM/glm.hpp>
#include <vector>
#include <Graphics/GLinit.h>


#define MAX_PARTICLES 120

class GameObject;

class AnimatedEmitter {

public:

	AnimatedEmitter(GLinit* glInit, const std::string& spritesheet, unsigned pxWidth, unsigned pxHeight, unsigned frames, float animDelay, bool billboarded, float sizeOfAnParticle);
	~AnimatedEmitter();

	void update(float dt);
	void addParticle(const glm::vec3& pos, const glm::vec3& velocity, float lifetime,  int numberOfParticles = 1);
	void addParticle(const glm::vec3& pos, const glm::vec3& rotation, const glm::vec3& velocity, float lifetime, int numberOfParticles = 1);
	void addParticle(GameObject* parent, const glm::vec3& offset, int numberOfParticles = 1);
	void addParticle(GameObject* parent, const glm::vec3& offset, const glm::vec3& rotation, int numberOfParticles = 1);

	const GLuint getVAO() const;
	const GLuint getTextureID() const;
	const unsigned int getNumberOfParticles() const;
	const unsigned int getNumberOfAnimationFrames() const;
	const bool isBillBoarded() const;

private:
	struct Particle {
		glm::vec3 velocity;
		glm::vec3 center;
		glm::vec3 rotation;
		glm::vec3 offset;
		glm::vec4 color;
		float lifetime;
		float initialLifetime;
		int animState;
		float animTimer;
		glm::mat4 modelMatrix;
		bool immortal;
		GameObject* parent;

		Particle()
			: center(0.0f), velocity(0.0f), rotation(0.0f), offset(0.0f),
			color(1.0f), lifetime(0.0f), initialLifetime(0.f),
			modelMatrix(1.0f), immortal(false), parent(nullptr),
			animState(0), animTimer(0.0f)
		{ }

	};

	GLuint m_textureID = -1;
	unsigned int m_nrOfParticles = 0;
	unsigned int m_lastUnusedParticle = 0;

	std::vector<Particle> m_particles;
	std::vector<glm::vec3> m_centerPosBuffer;
	std::vector<glm::vec4> m_colorBuffer;
	std::vector<glm::mat4> m_modelMatrixBuffer;
	std::vector<float> m_animationIndices;

	GLuint m_vao;
	GLuint m_verticesVBO;
	GLuint m_uvVBO;
	GLuint m_centerVBO;
	GLuint m_colorVBO;
	GLuint m_animationStateVBO;
	GLuint m_modelMatrixVBO;

	GLfloat m_vertex_buffer_data[12];
	GLfloat m_uv_buffer_data[8] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	bool m_billboarded;
	
	// Animation props
	float m_animDelay;
	int m_pxWidth;
	int m_pxHeight;
	int m_frames;
	float m_indx = 0;

private:
	void setupGraphicBuffers();
	void updateBuffers();
	size_t getFirstUnusedParticle();

};



#endif
