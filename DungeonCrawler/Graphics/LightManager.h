#ifndef _LIGHTMANAGER_H
#define _LIGHTMANAGER_H
#include <GL/glew.h>
#include "GLM/glm.hpp"
#include <vector>
#include "GameObjects/GameObjectManager.h"
#include "Shader.h"

class LightManager {

public:
	LightManager();
	~LightManager();

	void update(float dt);
	
	void addLight(const glm::vec3& position, const glm::vec3& color, const float& radius, GameObjectManager* gameObjectManager = nullptr);
	const int getNumberOfLights() const;
	
	void setSun(const glm::vec3& position, const glm::vec3& color);
	void setSunPosition(const glm::vec3& position);
	void setSunColor(const glm::vec3& color);

	void setPlayerLight(const glm::vec3& position, const glm::vec4& colorAndRange);
	void setPlayerLightPosition(const glm::vec3& position);
	/*Alpha value is the range!*/
	void setPlayerLightColorAndRange(const glm::vec4& colorAndRange);

	const glm::vec3& getSunColor() const;
	const glm::vec3& getSunPosition() const;
	const glm::vec3& getPlayerLightPosition() const;
	const glm::vec4& getPlayerLightColorAndRange() const;

private:
	
	GLuint m_ubo;
	float m_lightByteSize;

	glm::vec3 m_sunPosition;
	glm::vec3 m_sunColor;

	glm::vec3 m_playerLightPosition;
	glm::vec4 m_playerLightColorAndRange;

private:
	Light* m_playerLight;
	std::vector<Light> m_lights;
};

#endif
