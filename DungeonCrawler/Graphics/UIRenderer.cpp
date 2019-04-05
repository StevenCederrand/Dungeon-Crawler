#include "UIRenderer.h"

UIRenderer::UIRenderer(Camera* camera) {
	this->m_camera = camera;
	
}

UIRenderer::~UIRenderer() {
}

void UIRenderer::setupMenuButtons(MainMenu* menu) {
	this->m_mainMenu = menu;
}

void UIRenderer::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader* menuShader = ShaderMap::getShader("MenuShader");
	m_mainMenu->menuDraw();
	menuShader->unuse();
}
