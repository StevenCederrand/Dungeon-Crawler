#include "Powerups.h"

PowerUps::PowerUps(Mesh* mesh, Type type, float health, float damage, float speed, bool timed, glm::vec3 position) :
	GameObject(mesh, type)
{
	this->m_healthUp = health;
	this->m_damageUp = damage;
	this->m_speedUp = speed;
	this->m_type = type; 
	this->m_trigger = false;
	this->setPosition(position);
	this->m_timed = timed;
}

void PowerUps::update(float dt)
{

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
