#include "Walker.h"
#include "../GameObjectManager.h"
#include "../Player.h"
#include <list>
#include <System/Log.h>
#include <chrono>
#include <iostream>


Walker::Walker(Mesh * mesh, Type type, Room* room, const glm::vec3& position):
	GameObject(mesh, type)
{

	this->m_room = room;
	this->m_health = 1.f;
	this->m_speed = 3.f;
	this->m_damage = 1.f;
	this->m_isPlayerClose = false;
	this->m_type = type;
	this->m_amIAlive = true;
	setPosition(position);
	m_Astar = new AStar();

}

Walker::~Walker()
{
	delete m_Astar;
}


void Walker::update(float dt)
{
	calculatePath(dt);
	amIDead();
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

void Walker::calculatePath(float dt)
{
	m_AStarTimer += dt;

	// Runs every half second
	if (m_AStarTimer >= 0.5f) {
		m_AStarTimer = 0.0f;
		
		bool canRunAStar = true;

		// Get the cells that the player and the walker is standing on
		// and also checks if the error flag has been set, if so then A* can't be run, otherwise
		// it will crash when player is on a invalid cell or outside the cell system
		const GridCell& playerCell = m_room->getGrid()->getCell(getPlayerPosition().x, getPlayerPosition().z);
		if (m_room->getGrid()->failedGettingGridCell() || !playerCell.valid)
			canRunAStar = false;
		
		const GridCell& myCell = m_room->getGrid()->getCell(getPosition().x, getPosition().z);
		if (m_room->getGrid()->failedGettingGridCell())
			canRunAStar = false;

		// If there was no errors getting the cells then run A* star and get
		// the vector of nodes
		if (canRunAStar) {
			m_path = m_Astar->findPath(myCell, playerCell, m_room);
		}
	}

	
	// If there is nodes in the path vector then 
	// move to them and pop them when being close enough
	if (m_path.size() > 0)
	{
		int index = m_path.size() - 1;
		const Node& currentNode = m_path.at(index);
		const glm::vec3& myPos = getPosition();

		float xDir = currentNode.x - myPos.x;
		float zDir = currentNode.z - myPos.z;
		float length = sqrtf(xDir * xDir + zDir * zDir);
				
		glm::vec3 velocity = (glm::vec3(xDir, 0.f, zDir) / length) * m_speed * dt;
		
		// Move towards the node
		translate(velocity);

		// If being close enough then pop it from the path vector
		if (length <= m_room->getGrid()->getCellSize() * 0.5f)
		{
			m_path.erase(m_path.begin() + index);
		}

	}

}

