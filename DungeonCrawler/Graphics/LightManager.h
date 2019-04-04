#ifndef _LIGHTMANAGER_H
#define _LIGHTMANAGER_H
#include <GL/glew.h>
#include "GLM/glm.hpp"
#include <vector>

class LightManager {

public:
	LightManager();
	~LightManager();

	void update(float dt);
	
	void addLight(const glm::vec3& position, const glm::vec3& color, const float& radius);
private:
	
	GLuint m_ubo;
	float m_lightByteSize;

private:
	struct Light{
		glm::vec4 position;
		glm::vec4 color;
		float radius;
	};

	std::vector<Light*> m_lights;
};

#endif
