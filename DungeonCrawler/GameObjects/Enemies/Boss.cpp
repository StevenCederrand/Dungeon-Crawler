#include "Boss.h"
#include "../../Utility/Randomizer.h"
#include <System/Log.h>

Boss::Boss(Mesh* mesh, Type type, Room* room, const glm::vec3& position,ProjectileManager* projectileManager, Effects* effects):
	GameObject(mesh, type)
{
	m_projectileManager = projectileManager;
	m_effects = effects;
	this->setScale(glm::vec3(2.f, 2.f, 2.f));
	this->m_health = 30.f;
	this->m_speed = 12.f;
	this->m_damage = 1.f;
	this->m_isPlayerClose = false;
	this->m_type = type;
	this->m_amIAlive = true;
	this->m_room = room;
	this->m_sinTime = 0.f;
	this->m_sinAddTime = 20.f;
	m_attackCooldown = 0.f;
	
	m_shooting = false;
	m_shootingRechargeCooldown = 5.0f;
	m_currentShootingRechargeCooldown = m_shootingRechargeCooldown;
	m_shootingRounds = 0;
	m_shootingSpeed = 0.15f;
	m_shootingTimer = 0.0f;
	m_projectileSpeed = 25.0f;
	m_projectileDamage = 1.5f;

	setCollidable(true);
	setPosition(position);
	m_Astar = new AStar();
}

Boss::~Boss()
{
	delete m_Astar;
}

void Boss::update(float dt)
{
	lookAt(getPlayerPosition());
	updateHoverEffect(dt);
	updateBehaviour(dt);
	amIDead();
	updateCooldowns(dt);
}

bool Boss::meleeRange(float dt)
{
	if ((getDistanceToPlayer() <= 3.0f) && (m_attackCooldown <= 0.f))
	{
		m_attackCooldown = 1.f;
		return true;
	}
	return false;
}

void Boss::hit(const HitDescription& desc)
{
	Player* player = dynamic_cast<Player*>(desc.owner);
	m_health -= player->getDamage();
	amIDead();
}

Type Boss::getType()
{
	return this->m_type;
}

float Boss::getDamage() const
{
	return this->m_damage;
}

float Boss::getDistanceToPlayer() const
{
	float xDir = getPlayerPosition().x - getPosition().x;
	float zDir = getPlayerPosition().z - getPosition().z;
	float length = sqrtf(xDir * xDir + zDir * zDir);

	return length;
}

void Boss::amIDead()
{
	if (m_health <= 0)
	{
		m_amIAlive = false;
	}
}

bool Boss::getAliveStatus() const
{
	return m_amIAlive;
}

void Boss::bossWalkAnim(float dt)
{
	float sinCurve = (10 * sinf(m_sinTime));
	m_sinTime += (m_sinAddTime * dt);

	setRotation(glm::vec3(getRotation().x, getRotation().y, sinCurve));
	//setRotation(glm::vec3(sinCurve, getRotation().y, getRotation().z));
}

void Boss::updateCooldowns(float dt)
{
	if (m_attackCooldown > 0.f)
	{
		m_attackCooldown -= dt;
	}
}

void Boss::updateHoverEffect(float dt)
{
	m_hoverEffectTimer += dt;
	if (m_hoverEffectTimer >= 0.05f) {
		m_hoverEffectTimer = 0.0f;
		m_effects->addParticles("EnemyHoverEmitter", getPosition(), glm::vec3(Randomizer::single(-100.0f, 100.0f) / 100.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 100.0f), 1.0f, 1);
	}
}

void Boss::updateBehaviour(float dt)
{
	float lengthToPlayer = getDistanceToPlayer();

	if (m_currentShootingRechargeCooldown > 0.0f && !m_shooting) {
		m_currentShootingRechargeCooldown -= dt;
		if (lengthToPlayer > 2.5f) {
			calculatePath(dt, false, true);
			moveToTarget(dt);
			bossWalkAnim(dt);
		}

	}
	else
	{
		if (!m_shooting) {
			m_shooting = true;
			m_currentShootingRechargeCooldown = m_shootingRechargeCooldown;
			m_shootingRounds = Randomizer::single(15, 25);
			//setRotation(glm::vec3(getRotation().x, getRotation().y, 0.f));
		}
	}


	if (m_shooting)
	{
		m_shootingTimer += dt;

		if (m_shootingTimer >= m_shootingSpeed)
		{
			m_shootingTimer = 0.0f;

			m_path.clear();
			calculatePath(dt, true, false);

			if (m_path.size() > 0) {
				m_projectileManager->spawnProjectile(new Projectile(getPosition() + glm::vec3(0.0f, 2.0f, 0.0f), m_path, m_damage, m_projectileSpeed, m_room->getGrid()->getCellSize()));
				m_shootingRounds--;
				LOG_INFO("Rounds: " + std::to_string(m_shootingRounds));
			}

		}
	}

	if (m_shootingRounds <= 0 && m_shooting)
	{
		m_shooting = false;
		m_shootingRounds = 0;
		m_shootingTimer = 0.0f;
		m_path.clear();

	}


}

void Boss::calculatePath(float dt, bool ignoreTimer, bool occupy)
{
	bool canRunAStar = true;

	// Get the cell and occupy it
	const GridCell& myCell = m_room->getGrid()->getCell(getPosition().x, getPosition().z, occupy, this);
	if (m_room->getGrid()->failedGettingGridCell())
		canRunAStar = false;

	m_AStarTimer += dt;

	// Runs every 1/4 of a second
	if (m_AStarTimer >= 0.25f || ignoreTimer) {
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

void Boss::moveToTarget(float dt)
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
