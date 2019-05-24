#include "Powerups.h"
#include <Utility/Randomizer.h>

PowerUps::PowerUps(Mesh* mesh, Type type, float health, float damage, float speed, bool timed, glm::vec3 position, Effects* effects) :
	GameObject(mesh, type, position)
{
	this->m_healthUp = health;
	this->m_damageUp = damage;
	this->m_speedUp = speed;
	this->m_type = type; 
	this->m_trigger = false;
	this->m_timed = timed;

	this->m_particleSpawnDelay = 0.10f;
	this->m_particleSpawnTimer = 0.0f;
	this->m_particleVelUp = 2.5f;
	m_effect = effects;

}

PowerUps::~PowerUps()
{
}

void PowerUps::update(float dt)
{

	m_particleSpawnTimer += dt;

	if (m_particleSpawnTimer >= m_particleSpawnDelay)
	{
		m_particleSpawnTimer = 0.0f;

		if (m_healthUp > 0.0f)
		{
			m_effect->addParticles("HealthPickupEmitter", getPosition() + Randomizer::vec3(-0.5f, 0.5f, 0.0f, 0.0f, -0.5f, 0.5f), glm::vec3(0.0f, m_particleVelUp, 0.0f), 0.5f);
		}
		else if (m_speedUp > 0.0f)
		{
			m_effect->addParticles("SpeedPickupEmitter", getPosition() + Randomizer::vec3(-0.5f, 0.5f, 0.0f, 0.0f, -0.5f, 0.5f), glm::vec3(0.0f, m_particleVelUp, 0.0f), 0.5f);
		}
		else if (m_damageUp)
		{
			m_effect->addParticles("DamageBuffEmitter", getPosition() + Randomizer::vec3(-0.5f, 0.5f, 0.0f, 0.0f, -0.5f, 0.5f), glm::vec3(0.0f, m_particleVelUp, 0.0f), 0.5f);
		}

	}

}

void PowerUps::trigger()
{
	this->m_trigger = true;
}

bool PowerUps::powerTriggered()
{
	return m_trigger;
}

bool PowerUps::getTimed() const
{
	return m_timed;
}

glm::vec3 PowerUps::getBoost() const
{
	return glm::vec3(m_healthUp, m_damageUp, m_speedUp);
}

Type PowerUps::getType()
{
	return this->m_type;
}
