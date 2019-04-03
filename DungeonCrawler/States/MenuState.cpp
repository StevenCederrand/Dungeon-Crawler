#include "MenuState.h"
#include "StateManager.h"

#include "PlayState.h"
#include "System/Log.h"
#include "System/Input.h"
#include "Vendor/ImGui/imgui.h"
#include "../Audio/AudioEngine.h"


MenuState::MenuState() {

	AudioEngine::loadSound("MenuClickFine.wav", "MC");
	LOG_INFO("MenuState created");
}

MenuState::~MenuState()
{
	LOG_WARNING("MenuState destroyed");
}

void MenuState::update(float dt)
{
	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
	{
		m_stateManager->pushTemporaryState(new PlayState());
	}

	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
		AudioEngine::play("MC", 1.0f);
	}
	AudioEngine::update();

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
