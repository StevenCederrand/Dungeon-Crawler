#include "LightManager.h"
#include "ShaderMap.h"
#include "System/Log.h"
#include "Globals/Settings.h"
#include "MeshMap.h"
#include "GameObjects/LightSphere.h"

LightManager::LightManager()
{
	Shader* deferredShader = ShaderMap::getShader("LightPass");
	if (!deferredShader)
	{
		LOG_ERROR("Deferred Shader could not be found in shader map\nInitialize the shader before creating the light manager class!");
	}

	GLuint uniformBlockIndexDeferred = glGetUniformBlockIndex(deferredShader->getShaderID(), "Lights");
	glUniformBlockBinding(deferredShader->getShaderID(), uniformBlockIndexDeferred, 0);
	m_lightByteSize = sizeof(Light);

	
	glGenBuffers(1, &m_ubo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ubo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, int(Settings::getMaxLights() * m_lightByteSize), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ubo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

LightManager::~LightManager()
{
	glDeleteBuffers(1, &m_ubo);
}

void LightManager::update(float dt)
{
	
}

void LightManager::addLight(const glm::vec3 & position, const glm::vec3 & color, const float & radius, GameObjectManager* gameObjectManager)
{
	Light light;
	light.position = glm::vec4(position.x, position.y, position.z, 0.f);
	light.color = glm::vec4(color.x, color.y, color.z, radius);
	m_lights.emplace_back(light);

	if (gameObjectManager)
	{
		Mesh* sphere = MeshMap::getMesh("Sphere");
		GameObject* obj = new LightSphere(sphere, LIGHTSPHERE, position);
		obj->setCollidable(false);
		gameObjectManager->addGameObject(obj);

	}

	//(void*)m_lights.data()
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ubo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, (m_lights.size() - 1) * m_lightByteSize, m_lightByteSize, (void*)&light);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

const int LightManager::getNumberOfLights() const
{
	return int(m_lights.size());
}

const glm::vec3& LightManager::getSunColor() const
{
	return m_sunColor;
}

const glm::vec3& LightManager::getSunPosition() const
{
	return m_sunPosition;
}

const glm::vec3& LightManager::getPlayerLightPosition() const
{
	return m_playerLightPosition;
}

const glm::vec4& LightManager::getPlayerLightColorAndRange() const
{
	return m_playerLightColorAndRange;
}

void LightManager::setSun(const glm::vec3& position, const glm::vec3& color) {
	
	m_sunPosition = position;
	m_sunColor = color;
}

void LightManager::setSunPosition(const glm::vec3& position)
{
	m_sunPosition = position;
}

void LightManager::setSunColor(const glm::vec3& color)
{
	m_sunColor = color;
}

void LightManager::setPlayerLight(const glm::vec3& position, const glm::vec4& colorAndRange)
{
	m_playerLightPosition = position;
	m_playerLightColorAndRange = colorAndRange;
}

void LightManager::setPlayerLightPosition(const glm::vec3& position)
{
	m_playerLightPosition = position;
}

void LightManager::setPlayerLightColorAndRange(const glm::vec4& colorAndRange)
{
	m_playerLightColorAndRange = colorAndRange;
}
