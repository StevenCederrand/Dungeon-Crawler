#ifndef _MENUSTATE_H
#define _MENUSTATE_H
#include "State.h"

class MenuState : public State
{
public:
	MenuState();
	~MenuState();
	void update(float dt);
	void renderImGUI();
	void render();

private:

};

#endif
