#include "Shooter.h"
#include "../GameObjectManager.h"
#include "../Player.h"
#include <list>
#include <System/Log.h>

Shooter::Shooter(Mesh * mesh, Type type) :
	GameObject(mesh, type)
{
	this->m_health = 20.f;
	this->m_speed = 3.f;
	this->m_damage = 1.f;
	this->m_isPlayerClose = false;
	this->m_type = type;
	this->setPosition(glm::vec3(10.f, 0.f, 2.f));
	this->m_amIAlive = true;
}


void Shooter::update(float dt)
{

	amIDead();
}

void Shooter::hit(const HitDescription & desc)
{
	Player* player = dynamic_cast<Player*>(desc.owner);
	m_health -= player->getDamage();
	amIDead();
	LOG_WARNING("Shooter Health: " + std::to_string(m_health));
}

Type Shooter::getType()
{
	return this->m_type;
}

float Shooter::getDamage() const
{
	return this->m_damage;
}

void Shooter::amIDead()
{
	if (m_health <= 0)
	{
		m_amIAlive = false;
	}
}

bool Shooter::getAliveStatus() const
{
	return m_amIAlive;
}
