#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H
#include "State.h"

class PlayState : public State {
public:
	PlayState();
	~PlayState();

	void update(float dt);
	void renderImGUI();
	void render();

private:

};

#endif
