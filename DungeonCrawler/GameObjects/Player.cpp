#include "Player.h"
#include "System/Input.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "System/Log.h"
#include "Utility/Camera.h"

Player* Player::playerActive = nullptr;

Player::Player(Mesh * mesh) :
	GameObject(mesh)
{
	this->setPosition(glm::vec3(0.f, 0.f, 0.f));
	this->m_defaultSpeed = 7.f;
	this->m_speed = 7.0f;
	this->m_health = 5.f;
	this->m_damage = 0.f;
	this->m_debug = false;
	this->m_dash = 100.f;
	this->m_dashCd = false;
	this->m_timer = 0;
	this->m_shake = 0;
	this->m_shakeDir = glm::vec3(0.f, 0.f, 0.f);
}

void Player::update(float dt)
{
	if (Input::isKeyReleased(GLFW_KEY_Q))
	{
		m_debug = !m_debug;
	}
	if (!m_debug)
	{
		camPerspective();
		if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			dash();
		}
		if (Input::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			dash();
		}
		if (Input::isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			m_shake = 4;
			//shootProjectile();
			
		}
		move(dt);
		dashCd();
		screenShake();
	}
}

void Player::move(float dt)
{
	m_movementDirection = glm::vec3(0.f);

	rotatePlayer();
	if (Input::isKeyHeldDown(GLFW_KEY_W))
	{
		m_movementDirection.z =  -this->m_speed * dt;
	}
	if (Input::isKeyHeldDown(GLFW_KEY_A))
	{
		m_movementDirection.x = -this->m_speed * dt;
	}
	if (Input::isKeyHeldDown(GLFW_KEY_S))
	{
		m_movementDirection.z = this->m_speed * dt;
	}
	if (Input::isKeyHeldDown(GLFW_KEY_D))
	{
		m_movementDirection.x = this->m_speed * dt;
	}
	setVelocity(m_movementDirection);
	Camera::active->setToPlayer(getPosition(), m_shakeDir);
}

void Player::rotatePlayer()
{	
	glm::vec3 pos = Camera::active->getMouseWorldPos();

	glm::vec2 direction = glm::vec2(
		this->getPosition().x - pos.x,
		this->getPosition().z - pos.z);
	m_angle = glm::degrees(atan2f(direction.x, direction.y));

	setRotation(glm::vec3(0.f, m_angle, 0.f));
}

glm::vec3 Player::shakeDirection()const
{
	glm::vec3 lookDir = Camera::active->getMouseWorldPos() - getPosition();
	lookDir.y = 0;

	return glm::normalize(lookDir);
}

glm::vec3 Player::getPlayerPosition() const
{
	return getPosition();
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

void Player::shootProjectile()
{
	//glfwGetCursorPos(glfwGetCurrentContext(), &m_mousePos.x, &m_mousePos.y);

	screenShake();
}

void Player::screenShake()
{
	if (m_shake <= 0)
	{
		m_shakeDir *= 0.f;
	}
	if (m_shake > 0)
	{
		m_shakeDir = shakeDirection() * 0.25f;
	}
	if (m_shake > 2)
	{
		m_shakeDir = shakeDirection() * -0.25f;
	}
	if (m_shake > 0)
	{
		m_shake--;
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
