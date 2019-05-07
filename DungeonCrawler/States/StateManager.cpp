#include "StateManager.h"


StateManager::StateManager()
{
}


StateManager::~StateManager()
{
	clearStates();
}

void StateManager::popState()
{
	if (m_states.size() > 1){
		delete m_states[m_states.size() - 1];
		m_states.pop_back();
	}
	else{
		printf("Warning: Trying to remove the only state left in the stack!\n");
	}
}

void StateManager::pushTemporaryState(State * newState)
{
	newState->setManager(this);
	m_states.emplace_back(newState);
	m_stateWasChanged = true;
}

void StateManager::setState(State* newState)
{
	if (newState != nullptr) {
		clearStates();
		newState->setManager(this);
		m_states.emplace_back(newState);
		m_stateWasChanged = true;
	}
	else {
		printf("Warning: Trying to push a new state which is a nullptr!\n");
	}
}

void StateManager::update(float dt)
{
	if (!m_states.empty()) {
		m_states.back()->update(dt);
	}
}

void StateManager::renderImGUI()
{
	if (!m_states.empty()) {
		m_states.back()->renderImGUI();
	}
}

void StateManager::render()
{
	if (!m_states.empty()) {
		m_states.back()->render();
	}
}

const bool StateManager::wasStateChanged() const
{
	return m_stateWasChanged;;
}

void StateManager::resetWasStateChanged()
{
	m_stateWasChanged = false;
}

void StateManager::clearStates()
{
	for (size_t i = 0; i < m_states.size(); i++) {
		delete m_states[i];
	}
	m_states.clear();
}