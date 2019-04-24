#include "UIManager.h"
#include <Graphics/ShaderMap.h>
UIManager::UIManager(UICamera* camera)
{
	m_camera = camera;
	m_shader = ShaderMap::getShader("UIShader");
}

UIManager::~UIManager()
{
	for (size_t i = 0; i < m_uiElements.size(); i++)
	{
		delete m_uiElements[i];
	}
}

void UIManager::update(float dt)
{
	for (size_t i = 0; i < m_uiElements.size(); i++)
	{
		m_uiElements[i]->update(dt);
	}
}

void UIManager::render()
{
	m_shader->use();
	m_shader->setMat4("projectionMatrix", m_camera->getProjectionMatrix());
	m_shader->setMat4("viewMatrix", m_camera->getViewMatrix());

	for (size_t i = 0; i < m_uiElements.size(); i++)
	{
		m_shader->setMat4("modelMatrix", m_uiElements[i]->getModelMatrix());
		m_uiElements[i]->draw();
	}

	m_shader->unuse();
}

void UIManager::registerUIElement(UI* element)
{
	if (element)
	{
		m_uiElements.emplace_back(element);
	}
}
