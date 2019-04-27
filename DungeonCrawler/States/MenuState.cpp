#include "MenuState.h"
#include "StateManager.h"
#include "System/Log.h"
#include "System/Input.h"
#include "Vendor/ImGui/imgui.h"
#include "../Audio/AudioEngine.h"

#include <Graphics/ShaderMap.h>
#include <System/Application.h>

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
		//LOG_INFO("LOADING");
		std::this_thread::sleep_for(10ms);
	}
}

MenuState::MenuState() {
	this->m_camera = new UICamera();
	m_uiManager = new UIManager(m_camera);
	m_glInit = new GLinit();
	createUIElements();
	AudioEngine::loadSSO("Menu.sso");

}

MenuState::~MenuState()
{
	delete this->m_camera;
	delete m_glInit;
	delete m_uiManager;
}

void MenuState::createUIElements()
{
	m_logo = new Image(
		glm::vec2((float)Application::windowWidth * 0.5f, (float)Application::windowHeight - 125.f),
		glm::vec2(300.f, 200.f),
		m_glInit,
		"logo.png");

	m_playButton = new Button(
		glm::vec2((float)Application::windowWidth * 0.5f, (float)Application::windowHeight * 0.5),
		glm::vec2(150.f, 100.f),
		m_glInit,
		"playbtn_active.png",
		"playbtn_inactive.png");

	m_exitButton = new Button(
		glm::vec2((float)Application::windowWidth * 0.5f, (float)Application::windowHeight * 0.30),
		glm::vec2(150.f, 100.f),
		m_glInit,
		"exitbtn_active.png",
		"exitbtn_inactive.png");

	m_uiManager->registerUIElement(m_logo);
	m_uiManager->registerUIElement(m_playButton);
	m_uiManager->registerUIElement(m_exitButton);
}

void MenuState::update(float dt) {
	m_camera->update(dt);
	m_uiManager->update(dt);

	if (m_playButton->isPressed()) {
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

	if (m_exitButton->isPressed()) {
		glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
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


void MenuState::render() 
{
	m_uiManager->render();
}

