#ifndef _BUTTON_H
#define _BUTTON_H
#include <GLM/glm.hpp>
#include <Graphics/GLinit.h>


class Button {
public:
	Button(const glm::vec2& position, const glm::vec2& size, GLinit* glInit, const std::string& textureName);
	~Button();

	void update(float dt);
	void draw();
private:
	void setupGraphicsBuffers(GLfloat data[20]);

private:
	GLuint m_vao;
	GLuint m_vbo;

};


#endif
