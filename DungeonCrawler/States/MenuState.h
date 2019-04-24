#ifndef _MENUSTATE_H
#define _MENUSTATE_H
#include "State.h"
#include "../Utility/UICamera.h"
#include <UserInterface/UIManager.h>
#include <UserInterface/Button.h>
#include <UserInterface/Image.h>

class MenuState : public State
{
public:
	MenuState();
	~MenuState();
	void createUIElements();
	void update(float dt);
	void renderImGUI();
	void render();

private:
	UICamera* m_camera;
	GLinit* m_glInit;
	Image* m_logo;
	Button* m_playButton;
	Button* m_exitButton;
	UIManager* m_uiManager;
};

#endif
