#include "MenuState.h"
#include "StateManager.h"
#include "System/Log.h"
#include "System/Input.h"
#include "Vendor/ImGui/imgui.h"
#include "../Audio/AudioEngine.h"
#include <thread>
#include "PlayState.h"


using namespace std::chrono_literals;

void temp(bool& completed) {
	for (size_t i = 0; i < 1000; i++)
	{
		LOG_INFO(i);
	}
	completed = true;
}

void temp3(PlayState*& state, bool& completed) {
	if (state == nullptr) {
		state = new PlayState();
	}
	else {
		LOG_WARNING("STATE ALREADY INTIALIZED");
	}
	completed = true;
}

void temp2(const bool& completed) {
	while (!completed) {
		LOG_INFO("LOADING");
		std::this_thread::sleep_for(10ms);
	}
}

MenuState::MenuState() {
	this->m_menu = new MainMenu();
	this->m_camera = new Camera();
	this->m_renderer = new UIRenderer(this->m_camera);
	this->m_menu->insertButton(glm::vec2(0, 0), 2, 2);
	this->m_renderer->setupMenuButtons(this->m_menu);

	AudioEngine::loadSSO("Menu.sso");
	LOG_INFO("CREATED");
}

MenuState::~MenuState()
{
	delete this->m_camera;
	delete this->m_menu;
	delete this->m_renderer;
}

void MenuState::update(float dt) {
	//Force the player into the playstate
	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
		bool completed = false;

		//Have a side thread output text while it's loading
		std::thread b(temp2, std::ref(completed));

		//The main thread will then create the playstate
		PlayState* state = new PlayState();
		completed = true;
		b.join();

		AudioEngine::unloadSSO("Menu.sso");
		m_stateManager->pushTemporaryState(state); // this is where the loading happens

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