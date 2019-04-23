#include "Button.h"

Button::Button(const glm::vec2& position, const glm::vec2& size, GLinit* glInit, const std::string& textureName)
{
	GLfloat data[20] =
	{
		 -size.x, -size.y, 0.0f, 0.0f, 1.0f,
		  size.x, -size.y, 0.0f, 1.0f, 1.0f,
		 -size.x,  size.y, 0.0f, 0.0f, 0.0f,
		  size.x,  size.y, 0.0f, 1.0f, 0.0f
	};

	setupGraphicsBuffers(data);

}

Button::~Button()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void Button::update(float dt)
{
}

void Button::draw()
{
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, map.second->getTextureID());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glBindTexture(GL_TEXTURE_2D, NULL);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Button::setupGraphicsBuffers(GLfloat data[20])
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);

	// Buffer for vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  6 * sizeof(float), (void*) 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  6 * sizeof(float), (void*) (3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glBindVertexArray(NULL);
}
