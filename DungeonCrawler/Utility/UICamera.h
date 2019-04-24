#ifndef _UICAMERA_H
#define _UICAMERA_H
#include <GLM/glm.hpp>

class UICamera
{
public:
	UICamera();
	~UICamera();

	void update(float dt);

	const glm::mat4& getProjectionMatrix() const;
	const glm::mat4& getViewMatrix() const;
	
private:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_cameraUp;
	float m_fov;
	float m_farplane;
	float m_nearplane;
};

#endif
