#ifndef _MENUSTATE_H
#define _MENUSTATE_H
#include "State.h"
#include "../Utility/UICamera.h"
#include <UserInterface/Button.h>


class MenuState : public State
{
public:
	MenuState();
	~MenuState();
	void update(float dt);
	void renderImGUI();
	void render();

private:
	UICamera* m_camera;
	GLinit* m_glInit;
	Button* m_playButton;
	Button* m_exitButton;

};

#endif
