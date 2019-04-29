#include "Powerups.h"

PowerUps::PowerUps(Mesh* mesh, Type type, float health, float damage, float speed) :
	GameObject(mesh, type)
{
	this->m_healthUp = health;
	this->m_damageUp = damage;
	this->m_speedUp = speed;
	this->m_type = type; 
	this->m_trigger = false;
	this->setPosition(glm::vec3(2.f, 2.f, 2.f));
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
