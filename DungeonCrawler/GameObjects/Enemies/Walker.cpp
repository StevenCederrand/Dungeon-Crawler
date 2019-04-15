#include "Walker.h"
#include "../GameObjectManager.h"
#include "../Player.h"

Walker::Walker(Mesh * mesh)	:
	GameObject(mesh)
{
	this->m_health = 3.f;
	this->m_speed = 3.f;
	this->m_damage = 1.f;

	this->setPosition(glm::vec3(20.f, 0.f, 20.f));
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

