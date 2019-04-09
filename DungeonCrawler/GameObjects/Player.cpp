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
	this->m_defaultSpeed = 7.f;
	this->m_speed = 7.f;
	this->m_health = 5.f;
	this->m_damage = 0.f;
	this->m_debug = false;
	this->m_dash = 100.f;
	this->m_dashCd = false;
	this->m_timer = 0;
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
		camPerspective();
		dashCd();
		if (Input::isKeyPressed(GLFW_KEY_E))
		{
			dash();
		}
	}
}

void Player::move(float dt)
{
	rotatePlayer();
	Camera::active->setToPlayer(getPosition());
	if (Input::isKeyHeldDown(GLFW_KEY_W))
	{
		this->translate(glm::vec3(this->m_speed, 0.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_A))
	{
		this->translate(glm::vec3(0.f, 0.f, -this->m_speed) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_S))
	{
		this->translate(glm::vec3(-this->m_speed, 0.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_D))
	{
		this->translate(glm::vec3(0.f, 0.f, this->m_speed) * dt);
	}
}

void Player::rotatePlayer()
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

void Player::camPerspective()
{
	if (Input::isKeyPressed(GLFW_KEY_1))
	{
		Camera::active->setAngle(1);
	}
	if (Input::isKeyPressed(GLFW_KEY_2))
	{
		Camera::active->setAngle(2);
	}
	if (Input::isKeyPressed(GLFW_KEY_3))
	{
		Camera::active->setAngle(3);
	}
	if (Input::isKeyPressed(GLFW_KEY_4))
	{
		Camera::active->setAngle(4);
	}
	if (Input::isKeyPressed(GLFW_KEY_5))
	{
		Camera::active->setAngle(5);
	}
	if (Input::isKeyPressed(GLFW_KEY_6))
	{
		Camera::active->setAngle(6);
	}
}

void Player::dash()
{
	if (!m_dashCd)
	{
		setSpeed(m_dash);
		m_dashCd = !m_dashCd;
		m_timer = 30;
	}
}

void Player::dashCd()
{
	if (m_timer <= 0 && m_dashCd)
	{
		m_dashCd = !m_dashCd;
	}
	if (m_timer <= 28)
	{
		setSpeed(m_defaultSpeed);
	}
	if (m_timer > 0)
	{
		m_timer--;
	}
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
