#include "ScreenBlood.h"
#include <GLM/gtx/transform.hpp>

ScreenBlood::ScreenBlood(GLinit* glInit, Player* player)
{
	m_alpha = 0;
	m_player = player;
	m_modelMatrix = glm::mat4(1.0f);
	m_texturID = glInit->createTexture("HPBar_5.png", true, true);
	m_oldHealth = m_player->getHealth();
	m_takenDamage = false;
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
	m_takenDamage = false;
	float newHealth = m_player->getHealth();
	if (newHealth < m_oldHealth)
	{
		m_takenDamage = true;
		m_alpha = 1;
	}
	if (m_alpha >= 0)
	{
		m_alpha -= 2.5 * dt;
	}
	
	m_oldHealth = newHealth;


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

const bool& ScreenBlood::shouldRender() const
{
	return m_takenDamage;
}

const float& ScreenBlood::getAlpha() const
{
	return m_alpha;
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
