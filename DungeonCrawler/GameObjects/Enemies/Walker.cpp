#include "Walker.h"
#include "../GameObjectManager.h"
#include "../Player.h"
#include <list>
#include <System/Log.h>

Walker::Walker(Mesh * mesh, Type type)	:
	GameObject(mesh, type)
{
	this->m_health = 50.f;
	this->m_speed = 3.f;
	this->m_damage = 1.f;
	this->m_isPlayerClose = false;
	this->m_type = type;
	this->setPosition(glm::vec3(8.f, 0.f, 0.f));
	this->m_amIAlive = true;
}

void Walker::update(float dt)
{


	amIDead();
}

void Walker::move(float dt)
{
	
	
}

void Walker::moveToPlayer(float dt)
{
	glm::vec3 direction = getPlayerPosition() - getPosition();
	direction = glm::normalize(direction);
	 
}

void Walker::findPath(glm::vec3 startPos, glm::vec3 targetPos)
{

}

void Walker::hitPlayer()
{
	
}

void Walker::hit(const HitDescription & desc)
{
	Player* player = dynamic_cast<Player*>(desc.owner);
	m_health -= player->getDamage();
	amIDead();
	LOG_WARNING("Walker Health: " + std::to_string(m_health));
}

Type Walker::getType()
{
	return this->m_type;
}

float Walker::getDamage() const
{
	return this->m_damage;
}

void Walker::amIDead()
{
	if (m_health <= 0)
	{
		m_amIAlive = false;
	}
}

bool Walker::getAliveStatus() const
{
	return m_amIAlive;
}

