#ifndef _GAMEOVERSTATE_H
#define _GAMEOVERSTATE_H
#include "State.h"
#include "../Utility/UICamera.h"
#include <UserInterface/UIManager.h>
#include <UserInterface/Button.h>
#include <UserInterface/Image.h>


class GameOverState : public State
{

public:
	GameOverState();
	~GameOverState();

	void update(float dt);
	void renderImGUI();
	void render();

private:
	UICamera* m_camera;
	UIManager* m_manager;
	GLinit* m_glInit;
	Button* m_playAgain;
	Button* m_menu;

};

#endif // !_GAMEOVERSTATE_H


