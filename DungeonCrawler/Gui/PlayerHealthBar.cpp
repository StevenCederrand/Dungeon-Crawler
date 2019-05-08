#include "PlayerHealthBar.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

PlayerHealthBar::PlayerHealthBar(GLinit* glInit, Player* player)
{
	m_hpbar_5 = glInit->createTexture("HPBar_5.png", true, true);
	m_hpbar_4 = glInit->createTexture("HPBar_4.png", true, true);
	m_hpbar_3 = glInit->createTexture("HPBar_3.png", true, true);
	m_hpbar_2 = glInit->createTexture("HPBar_2.png", true, true);
	m_hpbar_1 = glInit->createTexture("HPBar_1.png", true, true);

	m_currentTexID = m_hpbar_5;
	m_player = player;
	m_size = 3.0f;

	GLfloat data[20] =
	{
		// Pos				// Uv
		 -0.5, -0.5, 0.0f, 0.0f, 1.0f,
		  0.5, -0.5, 0.0f, 1.0f, 1.0f,
		 -0.5,  0.5, 0.0f, 0.0f, 0.0f,
		  0.5,  0.5, 0.0f, 1.0f, 0.0f
	};

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

	m_playerMaxHealth = m_player->getHealth();
	m_healthStageValue = m_player->getHealth() / 5.0f;
}

PlayerHealthBar::~PlayerHealthBar()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteTextures(1, &m_currentTexID);
	glDeleteVertexArrays(1, &m_vao);
}

void PlayerHealthBar::update(float dt)
{
	float stage = std::ceilf(m_player->getHealth() / m_healthStageValue);

	if (stage > 4)
		m_currentTexID = m_hpbar_5;
	else if (stage > 3 && stage <= 4)
		m_currentTexID = m_hpbar_4;
	else if (stage > 2 && stage <= 3)
		m_currentTexID = m_hpbar_3;
	else if (stage > 1 && stage <= 2)
		m_currentTexID = m_hpbar_2;
	else
		m_currentTexID = m_hpbar_1;

	m_position = m_player->getPosition() + glm::vec3(0.0f, 0.1f, 0.0f);

	m_modelMatrix = glm::mat4(1.f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_size, m_size, m_size));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

}

const glm::mat4& PlayerHealthBar::getModelMatrix() const
{
	return m_modelMatrix;
}

const GLuint& PlayerHealthBar::getVAO() const
{
	return m_vao;
}

const GLuint& PlayerHealthBar::getTextureID() const
{
	return m_currentTexID;
}
