#include "Player.h"
#include "System/Input.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "System/Log.h"
#include "Utility/Camera.h"

Player::Player(Mesh * mesh) :
	GameObject(mesh)
{
	this->setPosition(glm::vec3(0.f, 0.f, 0.f));
	this->m_speed = 20.f;
	this->m_health = 5.f;
	this->m_damage = 0.f;
	this->m_debug = false;
}

void Player::update(float dt)
{
	if (Input::isKeyReleased(GLFW_KEY_Q))
	{
		m_debug = !m_debug;
	}
	if (!m_debug)
	{
		move(dt);
	}
}

void Player::move(float dt)
{
	m_movementDirection = glm::vec3(0.f);

	if (Input::isKeyHeldDown(GLFW_KEY_W))
	{
		m_movementDirection.x = m_speed * dt;
		//this->translate(glm::vec3(this->m_speed, 0.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_A))
	{
		m_movementDirection.z = -m_speed * dt;
		//this->translate(glm::vec3(0.f, 0.f, -this->m_speed) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_S))
	{
		m_movementDirection.x = -m_speed * dt;
		//this->translate(glm::vec3(-this->m_speed, 0.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_D))
	{
		m_movementDirection.z = m_speed * dt;
		//this->translate(glm::vec3(0.f, 0.f, this->m_speed) * dt);
	}

	//translate(m_movementDirection);
	setVelocity(m_movementDirection);
	rotatePlayer(dt);
	Camera::active->setToPlayer(getPosition());
}

void Player::rotatePlayer(float dt)
{	
	glfwGetCursorPos(glfwGetCurrentContext(), &m_mousePos.x, &m_mousePos.y);
	Ray ray = Camera::active->getRayFromScreen(m_mousePos.x, m_mousePos.y, 1280, 720);

	glm::vec3 planeNormal(0.f, 1.f, 0.f);
	float dis = glm::dot(-ray.pos, planeNormal) / (glm::dot(ray.dir, planeNormal) + 0.001f);

	glm::vec3 pos = ray.calcPoint(dis);

	glm::vec2 direction = glm::vec2(
		this->getPosition().x - pos.x,
		this->getPosition().z - pos.z);
	m_angle = glm::degrees(atan2f(direction.x, direction.y));

	setRotation(glm::vec3(0.f, m_angle, 0.f));
}

void Player::setSpeed(float speed)
{
	this->m_speed = speed;
}

void Player::setHealth(float health)
{
	this->m_health = health;
}

void Player::setDamage(float damage)
{
	this->m_damage = damage;
}

float Player::getSpeed() const
{
	return this->m_speed;
}

float Player::getHealth() const
{
	return this->m_health;
}

float Player::getDamage() const
{
	return this->m_damage;
}
