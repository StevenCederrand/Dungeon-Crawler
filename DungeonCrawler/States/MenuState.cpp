#include "MenuState.h"
#include "StateManager.h"

#include "PlayState.h"
#include "System/Log.h"
#include "System/Input.h"
#include "Vendor/ImGui/imgui.h"
#include "../Audio/AudioEngine.h"


MenuState::MenuState() {
	this->m_menu = new MainMenu();
	this->m_camera = new Camera();
	this->m_renderer = new UIRenderer(this->m_camera);
	this->m_menu->insertButton(glm::vec2(0, 0), 2, 2);
	this->m_renderer->setupMenuButtons(this->m_menu);

	AudioEngine::loadSSO("Menu.sso");
}

MenuState::~MenuState()
{
	delete this->m_camera;
	delete this->m_menu;
	delete this->m_renderer;
}

void MenuState::update(float dt) {

	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
	{
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
	this->m_renderer->render();
}
