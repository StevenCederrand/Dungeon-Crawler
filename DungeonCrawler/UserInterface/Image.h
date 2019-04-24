#ifndef _IMAGE_H
#define _IMAGE_H
#include "UI.h"
#include <Graphics/GLinit.h>

class Image : public UI {

public:
	Image(const glm::vec2& position, const glm::vec2& size, GLinit* glInit, const std::string& texture);
	~Image();
	
	void update(float dt);
	void draw();
	const glm::mat4& getModelMatrix() const;

private:
	void setupGraphicsBuffers(GLfloat data[]);

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_texID;
	
	glm::vec2 m_position;
	glm::vec2 m_size;

	glm::mat4 m_modelMatrix;

};

#endif