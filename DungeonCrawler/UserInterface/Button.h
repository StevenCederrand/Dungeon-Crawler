#ifndef _BUTTON_H
#define _BUTTON_H
#include <GLM/glm.hpp>
#include <Graphics/GLinit.h>
#include "UI.h"

class Button : public UI {
public:
	Button(const glm::vec2& position, const glm::vec2& size, GLinit* glInit, const std::string& activeTexture, const std::string& inactiveTexture);
	~Button();

	const bool isPressed() const;
	const glm::mat4& getModelMatrix() const;
	void update(float dt);
	void draw();
private:
	void setupGraphicsBuffers(GLfloat data[]);

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_texID_active;
	GLuint m_texID_inactive;
	GLuint m_texID_current;

	bool m_pressed;
	glm::vec2 m_position;
	glm::vec2 m_size;

	glm::mat4 m_modelMatrix;

};


#endif
