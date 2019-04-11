#include "Camera.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "System/Application.h"
#include "System/Input.h"
#include "System/Log.h"

Camera* Camera::active = nullptr;

Camera::Camera()
{
	m_position = glm::vec3(0.f, 10.f, -10.f);
	m_lookDirection = glm::vec3(0.f, -1.f, 1.f);

	m_yaw = 0.f;
	m_pitch = 0.f;
	m_distanceToOrbitPoint = 5.f;
	m_cameraRight = glm::vec3(1.f, 0.f, 0.f);
	m_cameraUp = glm::vec3(0.f, 1.0f, 0.f);
	m_cameraSpeed = 5.f;
	m_sensitivity = 0.1f;

	m_locked = false;
	m_debug = false;

	m_angle = 1;

	snapMouseToMiddle();
	setProjectionMatrix();
	calculateCameraAxis();
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
			if (Input::isKeyReleased(GLFW_KEY_Q))
			{
				m_debug = !m_debug;
			}
			if (m_debug)
			{
				move(dt);
				lookAround(dt);
			}
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
		m_position.y += m_lookDirection.y * m_cameraSpeed * dt;
		m_position.z += m_lookDirection.z * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_A))
	{
		m_position.x += m_lookDirection.z * m_cameraSpeed * dt;
		m_position.z -= m_lookDirection.x * m_cameraSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_S))
	{
		m_position.x -= m_lookDirection.x * m_cameraSpeed * dt;
		m_position.y -= m_lookDirection.y * m_cameraSpeed * dt;
		m_position.z -= m_lookDirection.z * m_cameraSpeed * dt;
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

const glm::vec3 Camera::getPosition() const
{
	return m_position;
}

const Ray Camera::getRayFromScreen(float x, float y, float w, float h) const
{
	glm::vec3 mouseWorld(glm::unProject(
		glm::vec3(x, h - y, 0.f),
		m_viewMatrix,
		m_projectionMatrix,
		glm::vec4(0, 0, w, h)
	));

	glm::vec3 dir(glm::normalize(mouseWorld - m_position));

	return Ray(m_position, dir);
}

const glm::vec3 Camera::getMouseWorldPos()
{
	glfwGetCursorPos(glfwGetCurrentContext(), &m_mousePos.x, &m_mousePos.y);
	Ray ray = Camera::active->getRayFromScreen(m_mousePos.x, m_mousePos.y, 1280, 720);

	glm::vec3 planeNormal(0.f, 1.f, 0.f);
	float dis = glm::dot(-ray.pos, planeNormal) / (glm::dot(ray.dir, planeNormal) + 0.001f);

	return ray.calcPoint(dis);
}

void Camera::setToPlayer(glm::vec3 playerPos, glm::vec3 shakeDir)
{
	if (!m_debug)
	{	
		if (m_angle == 1)
		{
			static glm::vec3 cameraOffset(0, 15, 15);
			m_position = cameraOffset + playerPos + shakeDir;
			m_lookDirection = glm::vec3(0.f, -1.f, -1.f);

			/*
			m_position.x = playerPos.x + shakeDir.x;
			m_position.z = playerPos.z + 10.f + shakeDir.z;
			m_position.y = playerPos.y + 10.f;
			m_lookDirection = glm::vec3(0.f, -1.f, -1.f);*/
		}
		if (m_angle == 2)
		{
			m_position.x = playerPos.x;
			m_position.z = playerPos.z - 10.f;
			m_position.y = playerPos.y + 10.f;
			m_lookDirection = glm::vec3(0.f, -1.f, 1.f);
		}
		if (m_angle == 3)
		{
			m_position.x = playerPos.x + 10.f;
			m_position.z = playerPos.z;
			m_position.y = playerPos.y + 10.f;
			m_lookDirection = glm::vec3(-1.f, -1.f, 0.f);
		}
		if (m_angle == 4)
		{
			m_position.x = playerPos.x - 10.f;
			m_position.z = playerPos.z;
			m_position.y = playerPos.y + 10.f;
			m_lookDirection = glm::vec3(1.f, -1.f, 0.f);
		}

		/*
		if (m_angle == 1)
		{
			m_position.x = playerPos.x - 10.f;
			m_position.z = playerPos.z;
			m_position.y = playerPos.y + 10.f;
			m_lookDirection = glm::vec3(1.f, -1.f, 0.f);
		}
		if (m_angle == 2)
		{
			m_position.x = playerPos.x + 10.f;
			m_position.z = playerPos.z;
			m_position.y = playerPos.y + 10.f;
			m_lookDirection = glm::vec3(-1.f, -1.f, 0.f);
		}
		if (m_angle == 3)
		{
			m_position.x = playerPos.x;
			m_position.z = playerPos.z - 10.f;
			m_position.y = playerPos.y + 10.f;
			m_lookDirection = glm::vec3(0.f, -1.f, 1.f);
		}
		if (m_angle == 4)
		{
			m_position.x = playerPos.x;
			m_position.z = playerPos.z + 10.f;
			m_position.y = playerPos.y + 10.f;
			m_lookDirection = glm::vec3(0.f, -1.f, -1.f);
		}*/





		if (m_angle == 5) //Cray cam
		{
			m_position.x = playerPos.x;
			m_position.z = playerPos.z + 3.f;
			m_position.y = playerPos.y + 3.f;
			m_lookDirection = glm::vec3(-1.f, -1.f, 1.f);
		}
		if (m_angle == 6) //Over Shoulder
		{
			m_position.x = playerPos.x - 2.f;
			m_position.z = playerPos.z + 2.f;
			m_position.y = playerPos.y + 3.f;
			m_lookDirection = glm::vec3(0.f, -0.5f, 1.f);
		}
	}
}

void Camera::setAngle(int angle)
{
	m_angle = angle;
}
