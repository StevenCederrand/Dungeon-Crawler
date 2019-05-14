#include "ScreenBlood.h"

ScreenBlood::ScreenBlood(GLinit* glInit, Player* player)
{
	m_player = player;
	m_texturID = glInit->createTexture("dead", true, true);

	GLfloat data[20] =
	{
		// Pos				// Uv
		 -1.0, -1.0, 0.0f, 0.0f, 1.0f,
		  1.0, -1.0, 0.0f, 1.0f, 1.0f,
		 -1.0,  1.0, 0.0f, 0.0f, 0.0f,
		  1.0,  1.0, 0.0f, 1.0f, 0.0f
	};

	setupBuffers();

}

ScreenBlood::~ScreenBlood()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteTextures(1, &m_texturID);
	glDeleteVertexArrays(1, &m_vao);
}

void ScreenBlood::update()
{
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
