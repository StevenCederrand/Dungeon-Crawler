#include "Player.h"
#include "System/Input.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "System/Log.h"
#include "Utility/Camera.h"
#include "Enemies/Walker.h"
#include "Enemies/Shooter.h"
#include "../Audio/AudioEngine.h"
#include "../Utility/Randomizer.h"
#include "Enemies/Boss.h"
#include "Powerups.h"
#include "../Vendor/Lodepng/lodepng.h"
#include "../Globals/Paths.h"
#include "cmath"
#define M_PI 3.14159265358979323846

Player::Player(Mesh* mesh, Type type) :
	GameObject(mesh, type)
{
	this->setPosition(glm::vec3(0.f, 0.f, 0.f));
	this->setScale(glm::vec3(0.65f, 0.65f, 0.65f));
	this->m_defaultSpeed = 7.f;
	this->m_speed = 7.0f;
	this->m_health = 1000000.f;
	this->m_maxHealth = 10.f;
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
	this->m_sinTime = 0.f;
	this->m_sinAddTime = 10.f; // 0.30 sekunder walk ljud - inte synkat

	this->m_spotlight = new Spotlight();
	this->m_spotlight->position = this->getPlayerPosition() + glm::vec3(0.0f, 1.0f, 0.0f);
	this->m_spotlight->radius = glm::cos(glm::radians(12.5f));
	this->m_spotlight->outerRadius = glm::cos(glm::radians(17.5f));
	this->m_flash = new Light();
	this->m_flash->color = glm::vec4(0.25, 0.25, 0.25, 0);
	this->m_flash->position = glm::vec4(getPlayerPosition(), 1.0f);

	this->m_chargeStance = false;
	this->m_shakeIntensity = 0.10f;
	this->m_chargeTimer = 1.f;
	this->m_weaponSlot = 1;
	this->m_spraying = false;
	this->m_type = type;
	//this->m_iframes = 0.f;
	this->m_pistolBullets = 6;
	this->m_reloadTime = 0.f;
	this->m_reloading = false;

	this->m_boostResetters = (glm::vec3(0.f, 0.f, 0.f));
	this->m_boostTimer = 0.f;
	this->m_poweredUp = false;

	this->setupSoundVector();



	for (size_t i = 0; i < 7; i++)
	{
		unsigned error = lodepng::decode(m_image[i], m_width, m_height, TexturePath + "AmmoBar" + std::to_string(i) +".png");
		if (error)
			LOG_WARNING(error);
	}
	m_data = new GLFWimage();

	m_data->width = 64;
	m_data->height = 64;
	m_data->pixels = m_image[6].data();
	m_cursor = glfwCreateCursor(m_data, 32, 32);
	glfwSetCursor(glfwGetCurrentContext(), m_cursor);
}

Player::~Player() {
	delete this->m_spotlight;
	delete this->m_flash;
	delete this->m_data;
}

void Player::update(float dt)
{
	// Start of by saying that the player is not shooting
	m_shooting = false;
	if (!Input::isMouseHeldDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		m_spraying = false;
	}
	//iframeCountdown(dt);

	if (Input::isKeyReleased(GLFW_KEY_Q))
	{
		m_debug = !m_debug;
	}
	
	if (!m_debug)
	{
		//weaponSwap();
		//if (m_weaponSlot == 1)
		//{
			shootAutomatic(dt);
			if ((m_pistolBullets <= 0) && (m_reloading == false) && (m_spraying == false))
			{
				m_reloading = true;
				AudioEngine::play("gun_reload", 1.0f);
				m_reloadTime = 4.f;
			}
			manualReload(dt);
		//}
		//if (m_weaponSlot == 2)
		//{
			//shootChargeShot(dt);
		//}
		powerUpCd(dt);
		move(dt);
		dashCd(dt);
		reloadCd(dt);
		screenShake(dt);
		spotlightHandler();
		
	}
}

void Player::hit(const HitDescription & desc)
{
	Type type = desc.owner->getType();
	/*if (m_iframes <= 0)
	{*/
	if (type == Type::WALKER)
	{
		Walker* walker = dynamic_cast<Walker*>(desc.owner);
		takeDamage(walker->getDamage());
	}
	if (type == Type::SHOOTER)
	{
		Shooter* shooter = dynamic_cast<Shooter*>(desc.owner);
		takeDamage(shooter->getDamage());
	}
	if (type == Type::BOSS)
	{
		Boss* boss = dynamic_cast<Boss*>(desc.owner);
		takeDamage(boss->getDamage());
	}
	/*	m_iframes = 2.f;
	}*/
	if (type == Type::POWERUPS)
	{
		PowerUps* powerUp = dynamic_cast<PowerUps*>(desc.owner);
		glm::vec3 boosts = powerUp->getBoost();
		if (powerUp->getTimed())
		{
			m_poweredUp = true;
			m_boostTimer = 5.f;
			m_boostResetters += boosts;
			m_health += boosts.x;
			m_automaticDamage += boosts.y;
			m_defaultSpeed += boosts.z;
		}
		else
		{
			if (boosts.x != 0)
			{
				setMaxHealth();
			}
			m_automaticDamage += boosts.y;
			m_defaultSpeed += boosts.z;
		}
		
	}

	LOG_WARNING("HP: " + std::to_string(m_health) + "DM: " + std::to_string(m_automaticDamage) + "SP: " + std::to_string(m_defaultSpeed));

}

Type Player::getType()
{
	return this->m_type;
}

void Player::move(float dt)
{
	m_movementDirection = glm::vec3(0.f);

	bool inMotion = false;
	rotatePlayer();
	if (Input::isKeyHeldDown(GLFW_KEY_W))
	{
		m_movementDirection.z =  -this->m_speed * dt;
		inMotion = true;
	}
	if (Input::isKeyHeldDown(GLFW_KEY_A))
	{
		m_movementDirection.x = -this->m_speed * dt;
		inMotion = true;
	}
	if (Input::isKeyHeldDown(GLFW_KEY_S))
	{
		m_movementDirection.z = this->m_speed * dt;
		inMotion = true;
	}
	if (Input::isKeyHeldDown(GLFW_KEY_D))
	{
		m_movementDirection.x = this->m_speed * dt;
		inMotion = true;
	}

	if (inMotion) {
		wobbleAnim(dt);
		AudioEngine::playOnce(m_walkSounds.at(1), 0.5f);
	}
	else
	{
		setRotation(glm::vec3(getRotation().x, getRotation().y, 0.f));
		m_sinTime = 0.f;
	}
	setVelocity(m_movementDirection);
	
	Camera::active->setToPlayer(getPosition(), m_shakeDir);
}

void Player::rotatePlayer()
{
	glm::vec3 pos = Camera::active->getMouseWorldPos();

	m_lookDirection = glm::vec3(
		pos.x - this->getPosition().x,
		0,
		pos.z - this->getPosition().z);
	m_angle = glm::degrees(atan2f(m_lookDirection.z, m_lookDirection.x));
	setRotation(glm::vec3(getRotation().x, -m_angle, getRotation().z));
}

const glm::vec3 & Player::getLookDirection() const
{
	return glm::normalize(m_lookDirection);
}

const float& Player::getAngle() const
{
	return m_angle;
}

glm::vec3 Player::shakeDirection()const
{
	glm::vec3 lookDir = Camera::active->getMouseWorldPos() - getPosition();
	lookDir.y = 0;

	return glm::normalize(lookDir);
}

Spotlight* Player::getSpotlight() {
	return m_spotlight;
}

Light* Player::getFlash() {
	return m_flash;
}

void Player::spotlightHandler() {
	this->m_spotlight->direction = this->getLookDirection();
	this->m_spotlight->position = this->getPosition();
	this->m_flash->position = glm::vec4(this->getPosition() + glm::vec3(0.0f, 2.f, 0.0f), 1.0);
}

void Player::setupSoundVector() {
	m_walkSounds.push_back("pl_walk");
	m_walkSounds.push_back("pl_walk-2");
	m_walkSounds.push_back("pl_walk-3");
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

	if ((Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT) && !m_spraying) || (Input::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT) && !m_spraying))
	{
		dash();
	}
	if (!m_reloading)
	{
		if (Input::isMouseHeldDown(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (m_canShoot && m_pistolBullets > 0)
			{
				shootProjectile(dt);
				m_shake = 0.05f;
				m_spraying = true;
				m_pistolBullets--;
				m_data->pixels = m_image[m_pistolBullets].data();
				m_cursor = glfwCreateCursor(m_data, 32, 32);
				glfwSetCursor(glfwGetCurrentContext(), m_cursor);
			}
			if (m_pistolBullets <= 0)
			{
				m_data->pixels = m_image[0].data();
				m_cursor = glfwCreateCursor(m_data, 32, 32);
				glfwSetCursor(glfwGetCurrentContext(), m_cursor);
				AudioEngine::playOnce("gun_click", 0.5f);
			}
		}
	}
	
	
	if (!m_canShoot)
	{
		m_shootingCooldown -= dt;
		if (m_shootingCooldown <= 0.f) {
			this->m_flash->color.a = 0;
			m_canShoot = true;
		}
	}

}

void Player::shootChargeShot(float dt)
{
	if ((Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT) && !m_chargeStance) || (Input::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT && !m_chargeStance)))
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

void Player::powerUpCd(float dt)
{
	if ((m_boostTimer <= 0) && (m_poweredUp))
	{
		m_health -= m_boostResetters.x;
		m_automaticDamage -= m_boostResetters.y;
		m_defaultSpeed -= m_boostResetters.z;
		m_poweredUp = false;
	}
	if (m_boostTimer > 0)
	{
		m_boostTimer -= dt;
	}
}

void Player::manualReload(float dt)
{
	if ((Input::isKeyPressed(GLFW_KEY_R)) && (m_reloading == false) && (m_pistolBullets < 6))
	{
		m_reloading = true;
		AudioEngine::play("gun_reload", 1.0f);
		m_reloadTime = 4.f;
	}
}

void Player::reloadCd(float dt)
{
	if ((m_reloadTime <= 0) && (m_reloading == true))
	{
		m_reloading = false;
		m_pistolBullets = 6;
		
		m_data->pixels = m_image[6].data();
		m_cursor = glfwCreateCursor(m_data, 32, 32);
		glfwSetCursor(glfwGetCurrentContext(), m_cursor);
	}
	if (m_reloadTime > 0)
	{
		m_reloadTime -= dt;
	}
}

void Player::shootProjectile(float dt)
{
	m_shootingCooldown = 0.25f;
	this->m_flash->color.a = 2.5f;
	m_canShoot = false;
	m_shooting = true;
	AudioEngine::play("pl_gun_shot", 0.8f);
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

void Player::wobbleAnim(float dt)
{
	float sinCurve = (10 * sinf(m_sinTime));
	m_sinTime += (m_sinAddTime * dt);
	
	setRotation(glm::vec3(getRotation().x, getRotation().y, sinCurve));
	//setRotation(glm::vec3(sinCurve, getRotation().y, getRotation().z));
}

void Player::setSpeed(float speed)
{
	this->m_speed = speed;
}

void Player::setHealth(float health)
{
	this->m_health = health;
}

void Player::setMaxHealth()
{
	this->m_health = this->m_maxHealth;
}

void Player::setDamage(float damage)
{
	this->m_damage = damage;
}

//void Player::iframeCountdown(float dt)
//{
//	if (m_iframes > 0.f)
//	{
//		m_iframes -= dt;
//	}
//}

void Player::setPlayerState(const EntityState& playerState) {
	this->playerState = playerState;
}

void Player::takeDamage(float damageRecieved)
{
	m_health = m_health - damageRecieved;
}

const EntityState& Player::getPlayerState() const
{
	return this->playerState;
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

int Player::getBulletCount() const
{
	return this->m_pistolBullets;
}

bool Player::isShooting() const
{
	return m_shooting;
}
