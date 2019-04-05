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

	//LOG_ERROR(std::to_string(m_mousePos.x)  +"    "+ std::to_string(m_mousePos.y));
	//this->translate(this->getPosition());
	/*if (m_mousePos.x >  0)
	{
		m_mousePos.x = m_mousePos.x * -1;
	}
	if (m_mousePos.y > 0)
	{
		m_mousePos.y = m_mousePos.y * -1;
	}*/
	glm::vec2 direction = glm::vec2(
		this->getPosition().x - m_mousePos.x,
		this->getPosition().z - m_mousePos.y);
	
	m_angle = glm::degrees(atan2f(direction.x, direction.y));
	
	


	//m_angle = this->getRotation().y + m_angle;
	/*if (m_angle < 0)
	{
		m_angle = m_angle * -1;
	}*/
	LOG_ERROR(m_angle);
	
	//this->setTranslateRotation(glm::vec3(0.f, m_angle, 0.f) * dt);
	setRotation(glm::vec3(0.f, m_angle, 0.f));
}
