#ifndef MAINMENU_h
#define MAINMENU_h
#include "MenuButton.h"
#include <vector>
class MainMenu {
public:
	MainMenu();
	~MainMenu();

	void insertButton(glm::vec2 position, float width, float height);

	void menuDraw();

private:
	std::vector<MenuButton> m_buttons;
};



#endif