#ifndef MENUBUTTON_h
#define MENUBUTTON_h
#include <GLM/glm.hpp>
#include <GL/glew.h>

class MenuButton {
public:
    MenuButton();
    MenuButton(glm::vec2 position, float width, float height);

    ~MenuButton();
	void initButton();

    void setButtonPosition(glm::vec2 position);
    void setWidth(float width);
    void setHeight(float height);
    void drawButton();

private:
	glm::vec2 m_position;
	float m_width;
    float m_height;

	unsigned int m_VAO;
	unsigned int m_VBO;

	float m_quadData[24] = {
		//VP			UV
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
};

#endif
