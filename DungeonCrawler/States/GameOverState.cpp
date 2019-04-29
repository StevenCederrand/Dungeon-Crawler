#include "GameOverState.h"
#include <System/Application.h>
#include "MenuState.h"
#include "Vendor/ImGui/imgui.h"


GameOverState::GameOverState()
{
	m_camera = new UICamera();
	m_manager = new UIManager(m_camera);
	m_glInit = new GLinit();

	m_playAgain = new Button(glm::vec2(Application::windowWidth * 0.5, Application::windowHeight * 0.7),
		glm::vec2(200.f, 200.f), m_glInit,
		"playbtn_active.png",
		"playbtn_inactive.png");

	m_menu = new Button(glm::vec2(Application::windowWidth * 0.5, Application::windowHeight * 0.3f),
		glm::vec2(200.f, 200.f), m_glInit,
		"exitbtn_active.png",
		"exitbtn_inactive.png");

	m_manager->registerUIElement(m_playAgain);
	m_manager->registerUIElement(m_menu);

}

GameOverState::~GameOverState()
{
	delete m_camera;
	delete m_glInit;
	delete m_manager;
}

void GameOverState::update(float dt)
{
	m_camera->update(dt);
	m_manager->update(dt);

	if (m_playAgain->isPressed())
	{
		m_stateManager->popState();
		return;
	}

	if (m_menu->isPressed())
	{
		MenuState* menu = new MenuState();
		m_stateManager->setState(menu);
		return;
	}


}

void GameOverState::renderImGUI()
{
	ImGui::Begin("GameOverState");

	ImGui::Text("Press right mouse to switch to the play state");

	ImGui::End();
}

void GameOverState::render()
{
	m_manager->render();
}
