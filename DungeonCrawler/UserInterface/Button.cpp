#include "Button.h"
#include <System/Input.h>
#include <System/Log.h>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <System/Application.h>

Button::Button(const glm::vec2& position, const glm::vec2& size, GLinit* glInit, const std::string& activeTexture, const std::string& inactiveTexture)
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

	m_texID_active = glInit->createTexture(activeTexture, true, true);
	m_texID_inactive = glInit->createTexture(inactiveTexture, true, true);

	m_modelMatrix = glm::mat4(1.f);
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(position, 1.f));
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(size, 0.f));
}

Button::~Button()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

const bool Button::isPressed() const
{
	return m_pressed;
}

const glm::mat4& Button::getModelMatrix() const
{
	return m_modelMatrix;
}

void Button::update(float dt)
{
	glm::vec2 mousePos = Input::getMousePosition();
	// flip the y coordinate
	mousePos.y = (float)Application::windowHeight - mousePos.y;
	
	m_pressed = false;

	if (mousePos.x >= m_position.x - m_size.x * 0.5f && mousePos.x <= m_position.x + m_size.x * 0.5f 
		&&
		mousePos.y >= m_position.y - m_size.y * 0.5f && mousePos.y <= m_position.y + m_size.y * 0.5f)
	{
		m_texID_current = m_texID_active;

		if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_1))
		{
			m_pressed = true;
		}

	}
	else
	{
		m_texID_current = m_texID_inactive;
	}

}

void Button::draw()
{
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texID_current);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindTexture(GL_TEXTURE_2D, NULL);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Button::setupGraphicsBuffers(GLfloat data[])
{

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);

	// Buffer for vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(float), (void*) (3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glBindVertexArray(NULL);
}
