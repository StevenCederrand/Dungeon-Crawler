#ifndef _MENUSTATE_H
#define _MENUSTATE_H
#include "State.h"
#include "../Main Menu/MainMenu.h"
#include "../Graphics/UIRenderer.h"
#include "../Utility/Camera.h"
#include "PlayState.h"
class MenuState : public State
{
public:
	MenuState();
	~MenuState();
	void update(float dt);
	void renderImGUI();
	void render();

private:
	MainMenu* m_menu;
	Camera* m_camera;
	UIRenderer* m_renderer;
};

#endif
