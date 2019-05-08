#include "Image.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

Image::Image(const glm::vec2& position, const glm::vec2& size, GLinit* glInit, const std::string& texture)
{
	m_position = position;
	m_size = size;

	GLfloat data[20] =
	{
		// Pos				// Uv
		 -0.5, -0.5, 0.0f, 0.0f, 1.0f,
		  0.5, -0.5, 0.0f, 1.0f, 1.0f,
		 -0.5,  0.5, 0.0f, 0.0f, 0.0f,
		  0.5,  0.5, 0.0f, 1.0f, 0.0f
	};

	setupGraphicsBuffers(data);

	m_texID = glInit->createTexture(texture, true, true);

	m_modelMatrix = glm::mat4(1.f);
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(position, 1.f));
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(size, 0.f));
}

Image::~Image()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteTextures(1, &m_texID);
	glDeleteVertexArrays(1, &m_vao);
}

void Image::update(float dt)
{
}

void Image::draw()
{
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindTexture(GL_TEXTURE_2D, NULL);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

const glm::mat4& Image::getModelMatrix() const
{
	return m_modelMatrix;
}

void Image::setupGraphicsBuffers(GLfloat data[])
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
