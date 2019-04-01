#ifndef _STATEMANAGER_H
#define _STATEMANAGER_H
#include <vector>
#include "State.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

	void popState();
	void pushTemporaryState(State* newState);
	void setState(State* state);

	void update(float dt);
	void renderImGUI();
	void render();

private:
	void clearStates();

private:
	std::vector<State*> m_states;
};

#endif