#include "LightManager.h"
#include "ShaderMap.h"
#include "System/Log.h"
#include "Globals/Settings.h"

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

	LOG_TRACE("Byte size of one light " + std::to_string(m_lightByteSize) + "b");

	glGenBuffers(1, &m_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	glBufferData(GL_UNIFORM_BUFFER, MaxLights * m_lightByteSize, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo, 0, MaxLights * m_lightByteSize);

}

LightManager::~LightManager()
{
	for (size_t i = 0; i < m_lights.size(); i++)
	{
		delete m_lights[i];
	}
}

void LightManager::update(float dt)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_lights.size() * m_lightByteSize, *m_lights.data());
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
}

void LightManager::addLight(const glm::vec3 & position, const glm::vec3 & color, const float & radius)
{
	Light* light = new Light();
	light->position = glm::vec4(position.x, position.y, position.z, 0.f);
	light->color = glm::vec4(color.x, color.y, color.z, 1.f);
	light->radius = radius;
	m_lights.emplace_back(light);
}