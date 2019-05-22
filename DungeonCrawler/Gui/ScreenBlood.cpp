#include "ScreenBlood.h"
#include <GLM/gtx/transform.hpp>

ScreenBlood::ScreenBlood(GLinit* glInit, Player* player)
{
	m_vao = 0;
	m_vbo = 0;

	m_alphaValue = 0;
	m_player = player;
	m_modelMatrix = glm::mat4(1.0f);
	m_texturID = glInit->createTexture("DamageOverlay.png", true, true);
	m_savedHealth = m_player->getHealth();
	GLfloat data[20] =
	{
		// Pos				// Uv
		 -1.0, -1.0, 0.0f, 0.0f, 1.0f,
		  1.0, -1.0, 0.0f, 1.0f, 1.0f,
		 -1.0,  1.0, 0.0f, 0.0f, 0.0f,
		  1.0,  1.0, 0.0f, 1.0f, 0.0f
	};
	setupBuffers(data);
}

ScreenBlood::~ScreenBlood()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteTextures(1, &m_texturID);
	glDeleteVertexArrays(1, &m_vao);
}

void ScreenBlood::update(float dt)
{
	float newHealth = m_player->getHealth();
	//if health now is lower than before set alpha to 1 (show the screenBlood)
	if (newHealth < m_savedHealth)
	{
		m_alphaValue = 1;
	}
	//lower alpha if it's over 0
	if (m_alphaValue >= 0 && newHealth > 2)
	{
		m_alphaValue -= 2.2 * dt;
	}

	m_savedHealth = newHealth;
	m_modelMatrix = glm::mat4(1.0f);
}

const glm::mat4& ScreenBlood::getModelMatrix() const
{
	return m_modelMatrix;
}

const GLuint& ScreenBlood::getVAO() const
{
	return m_vao;
}

const GLuint& ScreenBlood::getTextureID() const
{
	return m_texturID;
}

const float& ScreenBlood::getAlpha() const
{
	return m_alphaValue;
}

void ScreenBlood::setupBuffers(GLfloat data[])
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);

	// Buffer for vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glBindVertexArray(NULL);
}
