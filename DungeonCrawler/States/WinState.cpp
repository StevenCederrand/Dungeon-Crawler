#include "WinState.h"
#include <System/Application.h>
#include "Vendor/ImGui/imgui.h"

WinState::WinState() {
	this->m_glInit = new GLinit();
	this->m_uiCamera = new UICamera();
	this->m_uiManager = new UIManager(this->m_uiCamera);
	

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
	this->m_return = new Button(glm::vec2(Application::windowWidth * 0.3, Application::windowHeight * 0.2f),
		glm::vec2(400.f, 150.f), this->m_glInit,
		"mainmenubtn_active.png",
		"mainmenubtn_inactive.png");

	this->m_uiManager->registerUIElement(this->m_return);
}

void WinState::render() {
	this->m_uiManager->render();
}
	