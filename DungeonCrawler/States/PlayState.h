#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H
#include "State.h"
#include "Parser/Parser.h"
#include "Graphics/GLinit.h"

class PlayState : public State {
public:
	PlayState();
	~PlayState();

	void update(float dt);
	void renderImGUI();
	void render();

private:
	Parser* m_parser;
	GLinit* m_GLinit;
};

#endif
