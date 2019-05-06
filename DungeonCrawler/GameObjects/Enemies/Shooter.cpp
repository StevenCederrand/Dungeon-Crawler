#include "Shooter.h"
#include "../GameObjectManager.h"
#include "../Player.h"
#include <list>
#include <System/Log.h>

Shooter::Shooter(Mesh* mesh, Type type, Room* room, const glm::vec3& position, ProjectileManager* projectileManager) :
	GameObject(mesh, type)
{
	this->m_room = room;
	this->m_projectileManager = projectileManager;
	this->m_health = 1.0f;
	this->m_speed = 6.0f;
	this->m_damage = 1.0f;
	this->m_currentCastTime = 0.0f;
	this->m_maxShootingRange = 20.f;
	this->m_castTime = 5.0f;
	this->m_isPlayerClose = false;
	this->m_type = type;
	this->m_amIAlive = true;
	setPosition(position);
	m_Astar = new AStar();
}

Shooter::~Shooter()
{
	delete m_Astar;
}

void Shooter::update(float dt)
{

	float lengthToPlayer = glm::length(getPosition() - getPlayerPosition());
	this->lookAt(getPlayerPosition());
	// Use A* to go closer;
	if (lengthToPlayer > m_maxShootingRange)
	{
		// use A*
		calculatePath(dt, false);
		moveToTarget(dt);
	}
	else
	{
		// Clear path because he can just stand here and shoot

		m_path.clear();

		if (!m_castingSpell){
			m_castingSpell = true;
		}
	}

	if (m_castingSpell)
	{
		m_currentCastTime += dt;

		if (m_currentCastTime >= m_castTime)
		{
			// Find a path using A* and create some kind of projectile that travels that path
			m_castingSpell = false;
			m_currentCastTime = 0.0f;
			calculatePath(dt, true);
			LOG_TRACE("BOOM");
			m_projectileManager->spawnProjectile(new Projectile(getPosition(), m_path, m_damage, 12.0f, m_room->getGrid()->getCellSize()));
		}
	}

	
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

void Shooter::calculatePath(float dt, bool ignoreTimer)
{

	bool canRunAStar = true;

	// Get the cell and occupy it
	const GridCell& myCell = m_room->getGrid()->getCell(getPosition().x, getPosition().z, true, this);
	if (m_room->getGrid()->failedGettingGridCell())
		canRunAStar = false;

	m_AStarTimer += dt;

	// Runs every half second
	if (m_AStarTimer >= 1.f || ignoreTimer) {
		m_AStarTimer = 0.0f;

		// Get the cells that the player and the walker is standing on
		// and also checks if the error flag has been set, if so then A* can't be run, otherwise
		// it will crash when player is on a invalid cell or outside the cell system
		const GridCell& playerCell = m_room->getGrid()->getCell(getPlayerPosition().x, getPlayerPosition().z);
		if (m_room->getGrid()->failedGettingGridCell() || !playerCell.valid)
			canRunAStar = false;


		// If there was no errors getting the cells then run A* star and get
		// the vector of nodes
		if (canRunAStar) {
			m_path = m_Astar->findPath(this, myCell, playerCell, m_room);
		}
	}

}

void Shooter::moveToTarget(float dt)
{
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

		glm::vec3 velocity = (glm::vec3(xDir, 0.0f, zDir) / length) * m_speed * dt;
		
		// Move towards the node
		translate(velocity);

		// If being close enough then pop it from the path vector
		if (length <= m_room->getGrid()->getCellSize() * 0.5f)
		{
			m_path.erase(m_path.begin() + index);
		}

	}
}
