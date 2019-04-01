#include "PlayState.h"
#include "Vendor/ImGui/imgui.h"

#include "System/Log.h"
#include "System/Input.h"
#include "StateManager.h"


PlayState::PlayState()
{
	LOG_INFO("PlayState created");
}

PlayState::~PlayState()
{
	LOG_WARNING("PlayState destroyed");
}

void PlayState::update(float dt)
{
	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
	{
		m_stateManager->popState();
	}
}


void PlayState::renderImGUI()
{
	ImGui::Begin("PlayState");

	ImGui::Text("Press right mouse to switch back to menu state");

	ImGui::End();
}

void PlayState::render()
{
}
