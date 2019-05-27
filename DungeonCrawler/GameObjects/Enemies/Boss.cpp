#include "Boss.h"
#include "../../Utility/Randomizer.h"
#include <System/Log.h>

#include <GameObjects/GameObjectManager.h>
#include <Graphics/MeshMap.h>

Boss::Boss(Mesh* mesh, Type type, Room* room, const glm::vec3& position, GameObjectManager* gameObjectManager, ProjectileManager* projectileManager, Effects* effects, float timeBeforeSpawn):
	GameObject(mesh, type, position, timeBeforeSpawn)
{
	m_gameObjectManager = gameObjectManager;
	m_projectileManager = projectileManager;
	m_effects = effects;
	this->setScale(glm::vec3(2.f, 2.f, 2.f));
	this->m_health = 40.f;
	this->m_speed = 10.f;
	this->m_damage = 1.f;
	this->m_isPlayerClose = false;
	this->m_type = type;
	this->m_amIAlive = true;
	this->m_room = room;
	this->m_sinTime = 0.f;
	this->m_sinAddTime = 15.f;
	m_attackCooldown = 0.f;
	
	m_shooting = false;
	m_shootingRechargeCooldown = 5.0f;
	m_currentShootingRechargeCooldown = m_shootingRechargeCooldown;
	m_shootingRounds = 0;
	m_shootingSpeed = 0.15f;
	m_shootingTimer = 0.0f;
	m_projectileSpeed = 25.0f;
	m_projectileDamage = 1.5f;
	m_hasSpawned = false;

	m_spawnEnemiesCooldown = 20.0f;
	m_currentSpawnEnemiesCooldown = 10.0f;

	m_Astar = new AStar();

	m_effects->addAnimParticle("bossSummonCircle", position + glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.0f), timeBeforeSpawn);

}

Boss::~Boss()
{
	delete m_Astar;
}

void Boss::update(float dt)
{
	if (!m_hasSpawned)
	{
		m_hasSpawned = true;
		for (int i = 0; i < 15; i++)
			m_effects->addParticles("EnemySpawnEmitter", getPosition() + glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(Randomizer::single(-100.0f, 100.0f) / 10.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 10.0f), 1.0f, 1);
	}


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
	float sinCurve = ( 10*sinf(m_sinTime));
	m_sinTime += (m_sinAddTime * dt);
	
	setRotation(glm::vec3(getRotation().x, getRotation().y, sinCurve));
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
		m_effects->addParticles("EnemySpawnEmitter", getPosition() + glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(Randomizer::single(-100.0f, 100.0f) / 30.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 30.0F), 1.0f, 1);
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
			
			// Teleport

			for (int i = 0; i < 30; i++)
				m_effects->addParticles("EnemySpawnEmitter", getPosition() + glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(Randomizer::single(-100.0f, 100.0f) / 10.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 10.0f), 1.0f, 1);

			auto cell = m_room->getGrid()->getFreeRandomCell();
			setPosition(glm::vec3(cell.x, 0.0f, cell.z));

			for (int i = 0; i < 30; i++)
				m_effects->addParticles("EnemySpawnEmitter", getPosition() + glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(Randomizer::single(-100.0f, 100.0f) / 10.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 10.0f), 1.0f, 1);

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

	m_currentSpawnEnemiesCooldown -= dt;
	if (m_currentSpawnEnemiesCooldown <= 0.0f)
	{
		m_currentSpawnEnemiesCooldown = m_spawnEnemiesCooldown;
		Mesh* enemyMesh = MeshMap::getMesh("Enemy");
		float timeBeforeSpawn = 3.5f;
		int spawnOffset = 15;
		//Spawn Melee Enemies
		for (int i = 0; i < 3; i++)
		{
			auto cell = m_room->getGrid()->getFreeRandomCell();

			GameObject* enemy = new Walker(enemyMesh, WALKER, this->m_room, glm::vec3(cell.x, 0.0f, cell.z),
				m_effects, timeBeforeSpawn);

			m_gameObjectManager->addGameObject(enemy);
		}
		//Spawn Ranged Enemies
		for (int i = 0; i < 2; i++)
		{
			auto cell = m_room->getGrid()->getFreeRandomCell();

			GameObject* enemy = new Shooter(enemyMesh, SHOOTER, this->m_room, glm::vec3(cell.x, 0.0f, cell.z),
				m_projectileManager, m_effects, timeBeforeSpawn);

			m_gameObjectManager->addGameObject(enemy);
		}
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
