#include "Camera.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "System/Application.h"
#include "System/Input.h"
#include "System/Log.h"

Camera::Camera()
{
	m_position = glm::vec3(0.f, 0.f, 0.f);
	m_lookDirection = glm::vec3(0.f, 0.f, -1.f);
	m_yaw = 0.f;
	m_pitch = 0.f;
	m_distanceToOrbitPoint = 5.f;
	m_cameraRight = glm::vec3(1.f, 0.f, 0.f);
	m_cameraUp = glm::vec3(0.f, 1.0f, 0.f);
	m_cameraSpeed = 5.f;
	m_sensitivity = 0.1f;
	m_locked = false;

	snapMouseToMiddle();
	setProjectionMatrix();
}

Camera::~Camera()
{
}

void Camera::update(float dt)
{
	if (Input::isWindowFocused())
	{
		if (!m_locked)
		{
			move(dt);
			lookAround(dt);
			calculateCameraAxis();
			setViewMatrix();
		}
	}
}

void Camera::calculateCameraAxis()
{
	m_cameraRight = glm::normalize(glm::cross(m_lookDirection, glm::vec3(0.f, 1.0f, 0.f)));
	m_cameraUp = glm::cross(m_cameraRight, m_lookDirection);
}

void Camera::lookAround(float dt)
{
	glfwGetCursorPos(glfwGetCurrentContext(), &m_currentMousePosition.x, &m_currentMousePosition.y);
	glm::vec2 vel = m_currentMousePosition - m_lastMousePosition;
	snapMouseToMiddle();

	m_yaw += vel.x * m_sensitivity;
	m_pitch -= vel.y * m_sensitivity;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	else if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	glm::vec3 lookAt = glm::vec3(1.f);
	lookAt.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	lookAt.y = glm::sin(glm::radians(m_pitch));
	lookAt.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));;
	m_lookDirection = glm::normalize(lookAt);
}

void Camera::move(float dt)
{
	if (Input::isKeyHeldDown(GLFW_KEY_W))
	{
		m_position.x += m_lookDirection.x * m_cameraSpeed * dt;
		m_position.z += m_lookDirection.z * m_cameraSpeed * dt;
		m_position.y += m_lookDirection.y * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_A))
	{
		m_position.x += m_lookDirection.z * m_cameraSpeed * dt;
		m_position.z -= m_lookDirection.x * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_S))
	{
		m_position.x -= m_lookDirection.x * m_cameraSpeed * dt;
		m_position.z -= m_lookDirection.z * m_cameraSpeed * dt;
		m_position.y -= m_lookDirection.y * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_D))
	{
		m_position.x -= m_lookDirection.z * m_cameraSpeed * dt;
		m_position.z += m_lookDirection.x * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_SPACE))
	{
		m_position.y += m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_LEFT_CONTROL))
	{
		m_position.y -= m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_LEFT_SHIFT))
	{
		m_cameraSpeed = 50.f;
	}
	else
	{
		m_cameraSpeed = 5.f;
	}

	//LOG_TRACE("" + std::to_string(m_position.x) + ", " + std::to_string(m_position.y) + ", " + std::to_string(m_position.z));

}

void Camera::snapMouseToMiddle()
{
	glfwSetCursorPos(glfwGetCurrentContext(),(float)Application::windowWidth / 2.f, (float)Application::windowHeight / 2.f);
	glfwGetCursorPos(glfwGetCurrentContext(), &m_currentMousePosition.x, &m_currentMousePosition.y);
	m_lastMousePosition = m_currentMousePosition;
}

void Camera::setDistanceToOrbitPoint(float distance)
{
	m_distanceToOrbitPoint = distance;
}

void Camera::setProjectionMatrix()
{
	m_projectionMatrix = glm::perspective(glm::radians(FOV), (float)Application::windowWidth / (float)Application::windowHeight, NEAR_CLIPPING, FAR_CLIPPING);
}

void Camera::setViewMatrix()
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_lookDirection, m_cameraUp);
}

void Camera::setToOrbit(bool shouldOrbit)
{
	m_locked = shouldOrbit;
}

const glm::mat4 Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

const glm::mat4 Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}
