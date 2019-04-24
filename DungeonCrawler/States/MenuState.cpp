#include "MenuState.h"
#include "StateManager.h"

#include "PlayState.h"
#include "System/Log.h"
#include "System/Input.h"
#include "Vendor/ImGui/imgui.h"
#include "../Audio/AudioEngine.h"
#include <Graphics/ShaderMap.h>

#include <System/Application.h>

MenuState::MenuState() {
	this->m_camera = new UICamera();
	m_glInit = new GLinit();
	
	m_playButton = new Button(
		glm::vec2((float)Application::windowWidth * 0.5f, (float)Application::windowHeight * 0.75),
		glm::vec2(300.f, 200.f),
		m_glInit,
		"playbtn_active.png",
		"playbtn_inactive.png");

	m_exitButton = new Button(
		glm::vec2((float)Application::windowWidth * 0.5f, (float)Application::windowHeight * 0.25),
		glm::vec2(300.f, 200.f),
		m_glInit,
		"exitbtn_active.png",
		"exitbtn_inactive.png");


	AudioEngine::loadSSO("Menu.sso");

}

MenuState::~MenuState()
{
	delete this->m_camera;
	delete m_glInit;
	delete m_playButton;
	delete m_exitButton;
}

void MenuState::update(float dt) {
	//Force the player into the playstate
	m_camera->update(dt);
	m_playButton->update(dt);
	m_exitButton->update(dt);

	if (m_playButton->isPressed()) {
		m_stateManager->pushTemporaryState(new PlayState());
	}

	if (m_exitButton->isPressed()) {
		glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
	}

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

void MenuState::render() 
{
	Shader* shader = ShaderMap::getShader("UIShader");
	shader->use();
	shader->setMat4("projectionMatrix", m_camera->getProjectionMatrix());
	shader->setMat4("viewMatrix", m_camera->getViewMatrix());
	
	shader->setMat4("modelMatrix", m_playButton->getModelMatrix());
	m_playButton->draw();
	
	shader->setMat4("modelMatrix", m_exitButton->getModelMatrix());
	m_exitButton->draw();
	
	shader->unuse();
}
