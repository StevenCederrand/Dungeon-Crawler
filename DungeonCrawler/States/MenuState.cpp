#include "MenuState.h"
#include "StateManager.h"

#include "PlayState.h"
#include "System/Log.h"
#include "System/Input.h"
#include "Vendor/ImGui/imgui.h"
#include "../Audio/AudioEngine.h"


MenuState::MenuState() {
	this->m_camera = new Camera();

	AudioEngine::loadSSO("Menu.sso");
	LOG_INFO("CREATED");
}

MenuState::~MenuState()
{
	delete this->m_camera;
}

void MenuState::update(float dt) {
	//Force the player into the playstate
	m_stateManager->pushTemporaryState(new PlayState());
	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
	{
		AudioEngine::unloadSSO("Menu.sso");
		m_stateManager->pushTemporaryState(new PlayState());

	}

	if (Input::isKeyReleased(GLFW_KEY_S)) {
		AudioEngine::play("LMouseClick", 1.0f);
	}

	if (Input::isKeyReleased(GLFW_KEY_ENTER)) {
		AudioEngine::playOnce("SystemStart", 1.0f);
	}
	AudioEngine::update();

}

void MenuState::renderImGUI()
{
	ImGui::Begin("MenuState");

	ImGui::Text("Press right mouse to switch to the play state");

	ImGui::End();
}

void MenuState::render() {
}
