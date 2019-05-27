#include "GameOverState.h"
#include <System/Application.h>
#include "MenuState.h"
#include "Vendor/ImGui/imgui.h"
#include <Globals/Settings.h>
#include <Audio/AudioEngine.h>

GameOverState::GameOverState()
{
	m_camera = new UICamera();
	m_manager = new UIManager(m_camera);
	m_glInit = new GLinit();

	m_logo = new Image(
		glm::vec2((float)Settings::getScreenWidth() * 0.5f, (float)Settings::getScreenHeight() * 0.8f),
		glm::vec2(300.f, 150.f),
		m_glInit,
		"logo.png");

	m_dead = new Image(
		glm::vec2((float)Settings::getScreenWidth() * 0.5f, (float)Settings::getScreenHeight() * 0.5f),
		glm::vec2(300.f, 100.f),
		m_glInit,
		"Dead-New.png");

	m_playAgain = new Button(glm::vec2(Settings::getScreenWidth() * 0.7, Settings::getScreenHeight() * 0.2f),
		glm::vec2(400.f, 150.f), m_glInit,
		"PlayAgain-Sel.png",
		"PlayAgain-Unsel.png");

	m_menu = new Button(glm::vec2(Settings::getScreenWidth() * 0.3, Settings::getScreenHeight() * 0.2f),
		glm::vec2(400.f, 150.f), m_glInit,
		"MainMenu-Sel.png",
		"MainMenu-UnSel.png");

	m_manager->registerUIElement(m_playAgain);
	m_manager->registerUIElement(m_menu);
	m_manager->registerUIElement(m_logo);
	m_manager->registerUIElement(m_dead);
	AudioEngine::loadSSO("Menu.sso");
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
	if (Input::isMousePressed(GLFW_MOUSE_BUTTON_1)) {
		AudioEngine::play("LMouseClick", 1.0f);
	}


	if (m_playAgain->isPressed())
	{
		AudioEngine::unloadSSO("Menu.sso");
		AudioEngine::loadSSO("Game.sso");
		m_stateManager->popState();
		return;
	}

	if (m_menu->isPressed())
	{
		AudioEngine::unloadSSO("Menu.sso");
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
