#include "Player.h"
#include "System/Input.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "System/Log.h"


Player::Player(Mesh * mesh) :
	GameObject(mesh)
{
	this->setPosition(glm::vec3(0.f, 0.f, 0.f));
	
}

void Player::update(float dt)
{
	move(dt);
}

void Player::move(float dt)
{
	if (Input::isKeyHeldDown(GLFW_KEY_I))
	{
		this->translate(glm::vec3(10.f, 0.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_J))
	{
		this->translate(glm::vec3(0.f, 0.f, -10.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_K))
	{
		this->translate(glm::vec3(-10.f, 0.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_L))
	{
		this->translate(glm::vec3(0.f, 0.f, 10.f) * dt);
	}
	rotatePlayer(dt);
}

void Player::rotatePlayer(float dt)
{	
	/*
	if (Input::isKeyHeldDown(GLFW_KEY_U))
	{
		this->setTranslateRotation(glm::vec3(0.f, 100.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_O))
	{
		this->setTranslateRotation(glm::vec3(0.f, -100.f, 0.f) * dt);
	}*/

	glfwGetCursorPos(glfwGetCurrentContext(), &m_mousePos.x, &m_mousePos.y);

	glm::vec2 direction = glm::vec2(
		this->getPosition().x - m_mousePos.x,
		this->getPosition().z - m_mousePos.y);
	
	m_angle = (atan2(direction.x, direction.y));
	a = glm::degrees(m_angle);

	LOG_ERROR(m_angle);
	
	setRotation(glm::vec3(0.f, a, 0.f));
}
