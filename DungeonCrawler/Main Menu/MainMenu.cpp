#include "MainMenu.h"

MainMenu::MainMenu() {
}

MainMenu::~MainMenu() {
}

void MainMenu::insertButton(glm::vec2 position, float width, float height) {
	MenuButton button = MenuButton(position, width, height);
	button.initButton();
	m_buttons.push_back(button);
}

void MainMenu::menuDraw() {
	for (size_t i = 0; i < m_buttons.size(); i++) {
		m_buttons.at(i).drawButton();
	}
}