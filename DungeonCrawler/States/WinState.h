#ifndef WINSTATE_h
#define WINSTATE_h
#include "State.h"
#include "../Utility/UICamera.h"
#include <UserInterface/UIManager.h>
#include <UserInterface/Button.h>
#include <UserInterface/Image.h>
#include "MenuState.h"


class WinState : public State {
public: 
	WinState();
	~WinState();

	void update(float dt);
	void render();
	void renderImGUI();

private:
	void registerUIButtons();

private:
	UICamera* m_uiCamera;
	UIManager* m_uiManager;
	GLinit* m_glInit;	
	
	Image* m_winner;

	Button* m_return; //Return button that is used to go back to the main menu
};


#endif