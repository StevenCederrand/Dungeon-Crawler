#ifndef _STATE_H
#define _STATE_H

class StateManager; // Empty

class State {

public:

	virtual ~State(){}
	virtual void update(float dt) = 0;
	virtual void renderImGUI() = 0;
	virtual void render() = 0;

	// This is set by the StateManager class
	inline void setManager(StateManager* manager) {
		m_stateManager = manager;
	}

protected:
	StateManager* m_stateManager;

};


#endif