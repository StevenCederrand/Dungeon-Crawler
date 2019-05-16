#include "WinState.h"
#include <System/Application.h>
#include <Globals/Settings.h>
#include "Vendor/ImGui/imgui.h"

WinState::WinState() {
	this->m_glInit = new GLinit();
	this->m_uiCamera = new UICamera();
	this->m_uiManager = new UIManager(this->m_uiCamera);
	this->m_winner = new Image(
		glm::vec2((float)Settings::getScreenWidth() * 0.5f, (float)Settings::getScreenHeight() * 0.8f),
		glm::vec2(300.f, 150.f),
		m_glInit,
		"winner.png");

	this->m_uiManager->registerUIElement(m_winner);
	this->registerUIButtons();
}

WinState::~WinState() {
	delete this->m_uiManager;
	delete this->m_uiCamera;
	delete this->m_glInit;
}

void WinState::update(float dt) {
	this->m_uiCamera->update(dt);
	this->m_uiManager->update(dt);

	if (this->m_return->isPressed()) {
		MenuState* menu = new MenuState();
		this->m_stateManager->setState(menu);
		return;
	}

}

void WinState::renderImGUI() {
	ImGui::Begin("Win State");

	ImGui::Text("Rob Escaped!");
	ImGui::Text("Credits: ");
	ImGui::Text("Steven, Alexander, Eric, Ludwig, Tobbias, Oliver");

	ImGui::End();
}

void WinState::registerUIButtons() {

	this->m_return = new Button(glm::vec2(Settings::getScreenWidth() * 0.3, Settings::getScreenHeight() * 0.2f),
		glm::vec2(400.f, 150.f), this->m_glInit,
		"MainMenu-Sel.png",
		"MainMenu-UnSel.png");

	this->m_uiManager->registerUIElement(this->m_return);
}

void WinState::render() {
	this->m_uiManager->render();
}
	