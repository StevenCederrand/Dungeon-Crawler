#ifndef _MENUSTATE_H
#define _MENUSTATE_H
#include "State.h"
#include "../Utility/Camera.h"
class MenuState : public State
{
public:
	MenuState();
	~MenuState();
	void update(float dt);
	void renderImGUI();
	void render();

private:
	Camera* m_camera;
};

#endif
