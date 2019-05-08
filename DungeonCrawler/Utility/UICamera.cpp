#include "UICamera.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <Globals/Settings.h>

UICamera::UICamera()
{
	m_position = glm::vec3(0.f, 0.f, 1.f);
	m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
	m_direction = glm::vec3(0.f, 0.f, -1.f);
	m_fov = 45.f;
	m_farplane = 50.f;
	m_nearplane = 0.1f;

	//m_projectionMatrix = glm::perspective(glm::radians(m_fov), (float)Application::windowWidth / (float)Application::windowHeight, m_nearplane, m_farplane);
	m_projectionMatrix = glm::ortho(
		0.0f,
		(float)Settings::getScreenWidth(),
		0.0f,
		(float)Settings::getScreenHeight());
}

UICamera::~UICamera()
{

}

void UICamera::update(float dt)
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_cameraUp);
}

const glm::mat4& UICamera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

const glm::mat4& UICamera::getViewMatrix() const
{
	return m_viewMatrix;
}
