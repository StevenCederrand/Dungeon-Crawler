#include "Powerups.h"

Powerups::Powerups(Mesh* mesh, Type type, float health, float damage, float speed) :
	GameObject(mesh, type)
{
	this->m_healthUp = health;
	this->m_damageUp = damage;
	this->m_speedUp = speed;
	this->m_type = type; 
	this->m_trigger = false;
}

void Powerups::update(float dt)
{

	trigger();
}

void Powerups::trigger()
{
	if (m_trigger == true)
	{
		
	}
}

bool Powerups::powerTrigger()
{
	return m_trigger;
}
