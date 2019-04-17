#include "Walker.h"
#include "../GameObjectManager.h"
#include "../Player.h"
#include <list>
#include <System/Log.h>

Walker::Walker(Mesh * mesh, Type type)	:
	GameObject(mesh, type)
{
	this->m_health = 100.f;
	this->m_speed = 3.f;
	this->m_damage = 1.f;
	this->m_isPlayerClose = false;
	this->m_type = type;
	this->setPosition(glm::vec3(5.f, 0.f, 0.f));
}

void Walker::update(float dt)
{
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
	LOG_TRACE(m_health);
}

float Walker::getDamage() const
{
	return this->m_damage;
}

