#include "MenuButton.h"

MenuButton::MenuButton() {

}

MenuButton::MenuButton(glm::vec2 position, float width, float height) {
    this->m_position = position;
    this->m_width = width;
    this->m_height = height;
}

MenuButton::~MenuButton() {

}

void MenuButton::initButton() {

	glGenVertexArrays(1, &this->m_VAO);
	glBindVertexArray(this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_quadData), &this->m_quadData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MenuButton::setButtonPosition(glm::vec2 position) {
    this->m_position = position;
}

void MenuButton::setWidth(float width) {
    this->m_width = width;
}

void MenuButton::setHeight(float height) {
    this->m_height = height;
}

void MenuButton::drawButton() {
	glBindVertexArray(this->m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
