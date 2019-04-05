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
		GameObject* obj = new LightSphere(sphere, position);
		gameObjectManager->addGameObject(obj);
	}

	if (m_ubo == 0)
		glDeleteBuffers(1, &m_ubo);
	
	glGenBuffers(1, &m_ubo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ubo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MaxLights * m_lightByteSize, (void*)m_lights.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ubo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

const int LightManager::getNumberOfLights() const
{
	return m_lights.size();
}

void LightManager::setSun(Shader* shader, glm::vec3 position, glm::vec3 color) {
	shader->use();
	shader->setVec3("sunColor", color);
	shader->setVec3("sunPosition", position);
	shader->unuse();

}
