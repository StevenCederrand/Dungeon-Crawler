#include "MenuState.h"
#include "StateManager.h"

#include "System/Input.h"
#include "Vendor/ImGui/imgui.h"

#include "PlayState.h"

MenuState::MenuState()
{

}

MenuState::~MenuState()
{
	
}

void MenuState::update(float dt)
{
	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
	{
		m_stateManager->pushTemporaryState(new PlayState());
	}
}

void MenuState::renderImGUI()
{
	ImGui::Begin("MenuState");

	ImGui::Text("Press right mouse to switch to the play state");

	ImGui::End();
}

void MenuState::render()
{
}
