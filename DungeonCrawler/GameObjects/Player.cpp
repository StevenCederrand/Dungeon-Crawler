#include "Player.h"
#include "System/Input.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "System/Log.h"
#include "Utility/Camera.h"
#include "Enemies/Walker.h"

Player::Player(Mesh* mesh, Type type) :
	GameObject(mesh, type)
{
	this->setPosition(glm::vec3(0.f, 0.f, 0.f));
	this->m_defaultSpeed = 7.f;
	this->m_speed = 7.0f;
	this->m_health = 200.f;
	this->m_damage = 1.f;
	this->m_automaticDamage = 1.f;
	this->m_chargeDamage = 10.f;
	this->m_unChargedDamage = 2.f;
	this->m_debug = false;
	this->m_dash = 100.f;
	this->m_dashCd = false;
	this->m_dashTimer = 0.f;
	this->m_shake = 0.f;
	this->m_shakeDir = glm::vec3(0.f, 0.f, 0.f);
	this->m_chargeStance = false;
	this->m_shakeIntensity = 0.10f;
	this->m_chargeTimer = 1.f;
	this->m_weaponSlot = 1;
	this->m_spraying = false;
	this->m_type = type;
}

void Player::update(float dt)
{
	// Start of by saying that the player is not shooting
	m_shooting = false;

	if (Input::isKeyReleased(GLFW_KEY_Q))
	{
		m_debug = !m_debug;
	}
	if (!m_debug)
	{
		weaponSwap();
		if (m_weaponSlot == 1)
		{
			shootAutomatic(dt);
		}
		if (m_weaponSlot == 2)
		{
			shootChargeShot(dt);
		}
		move(dt);
		dashCd(dt);
		screenShake(dt);
	}
}

void Player::hit(const HitDescription & desc)
{
	Type type = desc.owner->getType();
	if (type == Type::WALKER)
	{
		Walker* walker = dynamic_cast<Walker*>(desc.owner);
		m_health -= walker->getDamage();
	}
}

Type Player::getType()
{
	return this->m_type;
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

	m_lookDirection = glm::vec3(
		pos.x - this->getPosition().x,
		0.0f,
		pos.z - this->getPosition().z);
	m_angle = glm::degrees(atan2f(m_lookDirection.z, m_lookDirection.x));
	
	setRotation(glm::vec3(0.f, -m_angle, 0.f));
}

const glm::vec3 & Player::getLookDirection() const
{
	return glm::normalize(m_lookDirection);
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

void Player::weaponSwap()
{
	if (Input::isKeyPressed(GLFW_KEY_1))
	{
		m_weaponSlot = 1;
	}
	if (Input::isKeyPressed(GLFW_KEY_2))
	{
		m_weaponSlot = 2;
	}
}

void Player::shootAutomatic(float dt)
{
	m_damage = m_automaticDamage;
	if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT) && !m_spraying || Input::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT && !m_spraying))
	{
		dash();
	}
	if (Input::isMouseHeldDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (m_canShoot)
		{
			shootProjectile(dt);
			m_shake = 0.05f;
			m_spraying = true;
		}
	}
	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_LEFT))
	{
		m_spraying = false;
	}

	if (!m_canShoot)
	{
		m_shootingCooldown -= dt;

		if (m_shootingCooldown <= 0.f) {
			m_canShoot = true;
		}
	}
}

void Player::shootChargeShot(float dt)
{
	if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT) && !m_chargeStance || Input::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT && !m_chargeStance))
	{
		dash();
	}
	if (Input::isMouseHeldDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		chargeProjectile(dt);
	}
	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_LEFT))
	{
		releaseChargedProjectile(dt);
	}
}

void Player::dash()
{
	if (!m_dashCd)
	{
		setSpeed(m_dash);
		m_dashCd = !m_dashCd;
		m_dashTimer = 1.f;
	}
}

void Player::dashCd(float dt)
{
	if (m_dashTimer <= 0.f && m_dashCd)
	{
		m_dashCd = !m_dashCd;
	}
	if (m_dashTimer <= 0.98f)
	{
		setSpeed(m_defaultSpeed);
	}
	if (m_dashTimer > 0.f)
	{
		m_dashTimer -= dt;
	}
}

void Player::shootProjectile(float dt)
{
	m_shootingCooldown = 0.10f;
	m_canShoot = false;
	m_shooting = true;
	screenShake(dt);
}

void Player::chargeProjectile(float dt)
{
	m_chargeStance = true;
	m_speed = 1.f;
	if (m_chargeTimer <= 0)
	{
		m_damage = m_chargeDamage;
	}
	if (m_chargeTimer > 0)
	{
		m_damage = m_unChargedDamage;
	}
	if (m_chargeTimer >= 0)
	{
		m_chargeTimer -= dt;
	}
}

void Player::releaseChargedProjectile(float dt)
{
	m_chargeStance = false;
	shootProjectile(dt);
	m_shake = 0.05f;

	if (m_damage == m_chargeDamage)
	{
		m_shakeIntensity = 1.f;
	}
	m_speed = m_defaultSpeed;
	m_chargeTimer = 1.f;
}

void Player::screenShake(float dt)
{
	if (m_shake <= 0.f)
	{
		m_shakeDir *= 0.f;
		m_shakeIntensity = 0.1f;
	}
	if (m_shake > 0.f)
	{
		m_shakeDir = shakeDirection() * m_shakeIntensity;
	}
	if (m_shake > 0.025f)
	{
		m_shakeDir = shakeDirection() * -m_shakeIntensity;
	}
	if (m_shake > 0.f)
	{
		m_shake -= dt;
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

void Player::takeDamage(float damageRecieved)
{
	m_health = m_health - damageRecieved;
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

bool Player::isShooting() const
{
	return m_shooting;
}
