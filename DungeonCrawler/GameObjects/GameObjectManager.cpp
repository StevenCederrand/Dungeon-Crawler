#include "GameObjectManager.h"
#include "System/Log.h"
#include "Box.h"
#include <vector>
#include "../Audio/AudioEngine.h"
#include "../Globals/Helper.h"
#include "../System/Input.h"
#include "../Graphics/MeshMap.h"
#include "../Utility/Randomizer.h"
#include <Graphics/Renderer.h>

GameObjectManager::GameObjectManager(Effects* effects, ProjectileManager* projectileManager)
{
	m_effects = effects;
	m_projectileManager = projectileManager;
	m_broadPhaseBox = nullptr;
	m_player = nullptr;
	m_walkerDifficulty = 2;
	m_shooterDifficulty = 0;
}

GameObjectManager::~GameObjectManager()
{
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}
	m_gameObjects.clear();

	if(m_broadPhaseBox)
		delete m_broadPhaseBox;
}

void GameObjectManager::update(float dt)
{	
	//LOG_INFO(std::to_string(this->m_numberOfEnemies));
	handleDeadEnemies(dt);
	//------ Player collision broadphasebox ( Used to speed up collision checking against map ) ------
	if (m_broadPhaseBox)
		m_broadPhaseBox->setParentPosition(m_player->getPosition());

	
	//------ Player collision with map ------
	bool hasCollided = false;
	glm::vec3 newVel = glm::vec3(0);
	

	//------ Update player manually ------
	m_player->update(dt);
	m_player->updateModelMatrix();
	
	//------ Send player object to the renderer
	if (m_renderer != nullptr)
		m_renderer->prepareGameObject(m_player);

	//------ Boost Visualisation ------
	const glm::vec3& playerBoostVec = m_player->getBoostVector();
	static float BoostVisTimer = 0.0f;
	const float velUp = 1.0f;
	const float yOffset = 3.5f;

	BoostVisTimer -= dt;

	if (BoostVisTimer <= 0.0f)
	{
		BoostVisTimer = 0.10f;
		

		if (playerBoostVec.y > 0.0f)
		{
			glm::vec3 offset = Randomizer::vec3(-10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f) / 15.0f;
			offset.y = yOffset;
			m_effects->addParticles("DamageBuffEmitter", m_player->getPosition() + offset, glm::vec3(0.0f, velUp, 0.0f), 0.5f);
		}

		if (playerBoostVec.z > 0.0f)
		{
			glm::vec3 offset = Randomizer::vec3(-10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f) / 15.0f;
			offset.y = yOffset;
			m_effects->addParticles("SpeedPickupEmitter", m_player->getPosition() + offset, glm::vec3(0.0f, velUp, 0.0f), 0.5f);
		}

	}

	
	//------ Player ray used for shooting ------
	float rayLengthUntilCollision = -1.0f;
	glm::vec3 rayDirection = m_player->getLookDirection();
	GameObject* objectHit = nullptr;

	
	//------ Player current velocity ( Also used for collision ) ------
	newVel = m_player->getVelocity();

	//------ Add shooting particle effect ------
	if (m_player->isShooting()) {
		float xAngle = cosf(glm::radians(m_player->getAngle()));
		float zAngle = sinf(glm::radians(m_player->getAngle()));
		float offset = 0.5f;
		
		glm::vec3 playerPos = m_player->getPosition();
		glm::vec3 lookDir = m_player->getLookDirection();

		glm::vec3 posToSpawnParticle = playerPos + glm::vec3(xAngle, 2.5f, zAngle) + (offset * lookDir);
		m_effects->addParticles("GunFlareEmitter", posToSpawnParticle, glm::vec3(1.0f, 0.5f, 1.0f) * lookDir, 0.50f);
	}

	//------ Update all the game objects and check for collision 'n stuff ------
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		// Get the object, just convenient
		GameObject* object = m_gameObjects[i];

		// If it's the player then dont go further because the player
		// is manually updated above because the velocity of the player is needed
		// when doing the collision detection and handling below
		if (m_player == object) {
			continue;
		}


		if (object->getType() == ROOM || object->getType() == ROOM_EMPTY
			|| object->getType() == ROOM_BOSS) {
			this->roomManager(object);
		}

		// Update the object
		object->setPlayerPosition(m_player->getPosition());
		object->internalUpdate(dt);
		if (object->isSpawned()) {
			object->update(dt);
			object->updateModelMatrix();
			
			if (m_renderer != nullptr)
				m_renderer->prepareGameObject(object);

			handleEnemyAttacks(object, dt);
			// Handle collisions if there is any
			handlePlayerCollisionAgainstObjects(dt, object, newVel, hasCollided);

			// If player is shooting then handle it
			if (m_player->isShooting()) {
				handlePlayerShooting(dt, object, rayDirection, rayLengthUntilCollision, objectHit);
			}
		}
	}

	// Lastly we translate the player with the velocity that has been 
	// modyfied by the collision handling function
	m_player->translate(newVel);

	// Also here is the length we need to travel to get to the closest object the
	// player hit if he fired a gun. 
	// If the length is -1 then there was no intersection
	if (rayLengthUntilCollision != -1.0f)
	{
		glm::vec3 gunshotCollisionPoint = m_player->getPosition() + rayDirection * rayLengthUntilCollision;
		if (objectHit)
		{
			// --------MAYBE DYNAMIC CASY HERE TO CHECK IF WE HIT A ENEMY?--------
			bool hitEnemy = false;
			
			if (dynamic_cast<Box*>(objectHit)) {
			}
			if (dynamic_cast<Walker*>(objectHit)) {
				hitEnemy = true;
				objectHit->setHit();
			}
			if (dynamic_cast<Shooter*>(objectHit))
			{
				hitEnemy = true;
				objectHit->setHit();
			}
			if (dynamic_cast<Boss*>(objectHit))
			{
				hitEnemy = true;
				objectHit->setHit();
			}

			HitDescription desc;
			desc.player = m_player;
			objectHit->hit(desc);

			if (hitEnemy){
				AudioEngine::play("gun_impact_enemy", 0.6f);
				for (int i = 0; i < 10; i++) {
					m_effects->addParticles("BloodEmitter", objectHit->getPosition() + glm::vec3(0.0f, 1.0f, 0.0f),
						glm::vec3(Randomizer::single(-100.0f, 100.0f) / 25.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 25.0f), 0.25f);
				}
			}
			else{
				//AudioEngine::play("gun_impact_wall", 0.5f);
				m_effects->addParticles("WallSmokeEmitter", gunshotCollisionPoint, glm::vec3(0.0f, 5.0f, 0.0f), 0.2f, 5.f);
			}
		}
	}
}

void GameObjectManager::addGameObject(GameObject * gameObject)
{
	if (gameObject) {
		// If there is no player added then dynamic cast it and check if the object that
		// is going to be added in a few lines later is the player, if it is then construct
		// a broadphase box ( Used to make collision more efficient )
		if (!m_player)
		{
			m_player = dynamic_cast<Player*>(gameObject);
			// If this object is a player
			if (m_player)
			{
				constructPlayerBroadPhaseBox();
				m_player->setPlayerState(ROAMING);
			}
		}
		Type objectType = gameObject->getType();

		if (objectType == SHOOTER || objectType == WALKER || objectType == BOSS) {
			this->m_numberOfEnemies++;
		}
		//No matter the type of room, add it. 
		else if (objectType == ROOM || objectType == ROOM_EMPTY 
			|| objectType == ROOM_BOSS) {

			Room* room = dynamic_cast<Room*>(gameObject);
			this->m_rooms.push_back(room);
			m_roomsCleared.emplace_back(room); // show rooms
		}
		else if (objectType == DOOR) {
			m_doorIndex = m_gameObjects.size();
		}
		m_gameObjects.emplace_back(gameObject);
	}
}

void GameObjectManager::constructPlayerBroadPhaseBox()
{
	if(!m_broadPhaseBox)
		m_broadPhaseBox = new AABB();
	
	m_broadPhaseBox->setDimensions(
		m_player->getBoundingBoxes()[0]->getDimensions().x * 10.f,
		m_player->getBoundingBoxes()[0]->getDimensions().y * 10.f,
		m_player->getBoundingBoxes()[0]->getDimensions().z * 10.f);
	m_broadPhaseBox->setParentPosition(m_player->getPosition());

}

void GameObjectManager::setRendererRef(Renderer* renderer)
{
	m_renderer = renderer;
}

Player * GameObjectManager::getPlayer() const {

	return m_player;
}


const std::vector<GameObject*>& GameObjectManager::getGameObjects() const
{
	return m_gameObjects;
}

std::vector<GameObject*>* GameObjectManager::getVectorPointer()
{
	return &m_gameObjects;
}

bool GameObjectManager::gameFinished()
{
	if (this->m_gameFinished) {
		return true;
	}
	return false;
}

std::vector<Room*>& GameObjectManager::getClearedRooms()
{
	return m_roomsCleared;
}

void GameObjectManager::handlePlayerCollisionAgainstObjects(float dt, GameObject * object, glm::vec3& newVel, bool& hasCollided)
{
	// If the object is collidable then handle collision
	if (object->isCollidable())
	{
		AABB* playerBox = m_player->getBoundingBoxes()[0];
		const std::vector<AABB*> objectBoxes = object->getBoundingBoxes();

		for (size_t i = 0; i < objectBoxes.size(); i++) {
			AABB* objectBox = objectBoxes[i];

			if (m_broadPhaseBox->checkCollision(*objectBox))
			{
				
				float nx = 0.f, nz = 0.f;
				float collisionTime = playerBox->swepAABB(newVel, *objectBox, nx, nz);

				if (nx == 0)
					newVel.z *= std::min(collisionTime, 1.0f);
				else if (nz == 0)
					newVel.x *= std::min(collisionTime, 1.0f);

				if (collisionTime < 1.0f)
				{
					HitDescription desc;
					if (dynamic_cast<PowerUps*>(object))
					{
						m_powerup = dynamic_cast<PowerUps*>(object);

						if (m_powerup->getBoost().x != 0)
						{
							for (int jj = 0; jj < 25; jj++) {
								glm::vec3 offset = Randomizer::vec3(-10.0f, 10.0f, 0.0f, 0.0f, -10.0f, 10.0f) / 15.0f;
								offset.y = 1.0f;
								m_effects->addParticles("HealthPickupEmitter", m_player->getPosition() + offset, glm::vec3(0.0f, 2.0f, 0.0f), 1.50f);
							}
						}


						desc.powerUp = m_powerup;
						m_player->hit(desc);
						dynamic_cast<PowerUps*>(object)->trigger();

						
						continue;
					}
					float remainingTime = 1.0f - collisionTime;
					hasCollided = true;
					float dotprod = (newVel.x * nz + newVel.z * nx) * remainingTime;
					newVel.x = dotprod * nz;
					newVel.z = dotprod * nx;

					if (dynamic_cast<Walker*>(object))
					{
						Walker* walker = dynamic_cast<Walker*>(object);
						desc.walker = walker;
						m_player->hit(desc);
					}
					if (dynamic_cast<Shooter*>(object))
					{
						Shooter* shooter = dynamic_cast<Shooter*>(object);
						desc.shooter = shooter;
						m_player->hit(desc);
					}
					if (dynamic_cast<Boss*>(object))
					{
						Boss* boss = dynamic_cast<Boss*>(object);
						desc.boss = boss;
						m_player->hit(desc);
					}
				}
			}
		}
	}
}

void GameObjectManager::handlePlayerShooting(float dt, GameObject * object, const glm::vec3& rayDir, float& rayLengthUntilCollision, GameObject* & hitGameObject)
{
	if (object->isCollidable())
	{
		const std::vector<AABB*> objectBoxes = object->getBoundingBoxes();
		
		for (size_t i = 0; i < objectBoxes.size(); i++)
		{
			AABB* objectBox = objectBoxes[i];

			// if this is changed after the collision test then update the hit gameobject pointer (Copy it)
			float rayLengthBefore = float(rayLengthUntilCollision);

			// This function will alter the rayLengthUntilCollision if the ray is intersecting an object
			objectBox->checkCollisionWithRay(m_player->getPosition(), rayDir, rayLengthUntilCollision);

			if (rayLengthUntilCollision > 0.0f && (rayLengthBefore > rayLengthUntilCollision || rayLengthBefore < 0.f)) {
				hitGameObject = object;
			}
			else{
				// Object is further away so set the raylengthUntilCollision to what is was before
				rayLengthUntilCollision = rayLengthBefore;
			}

		}
	}
}

void GameObjectManager::handleDeadEnemies(float dt)
{
	
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		// Get the object, just convenient
		GameObject* object = m_gameObjects[i];
		
		if (dynamic_cast<Walker*>(object))
		{
			if (!dynamic_cast<Walker*>(object)->getAliveStatus())
			{
				for (int i = 0; i < 10; i++) {
					m_effects->addParticles("ProjectileExplosionEmitter", object->getPosition() + glm::vec3(0.0f, 2.5f, 0.0f),
						glm::vec3(Randomizer::single(-100.0f, 100.0f) / 25.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 25.0f), 0.50f);
				}

				this->m_numberOfEnemies--;
				delete m_gameObjects[i];
				m_gameObjects.erase(m_gameObjects.begin() + i);
				continue;
			}
		}

		if (dynamic_cast<Shooter*>(object))
		{
			if (!dynamic_cast<Shooter*>(object)->getAliveStatus())
			{
				for (int i = 0; i < 10; i++) {
					m_effects->addParticles("ProjectileExplosionEmitter", object->getPosition() + glm::vec3(0.0f, 2.5f, 0.0f),
						glm::vec3(Randomizer::single(-100.0f, 100.0f) / 25.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 25.0f), 0.50f);
				}

				this->m_numberOfEnemies--;
				delete m_gameObjects[i];
				m_gameObjects.erase(m_gameObjects.begin() + i);
				continue;
			}
		}
		if (dynamic_cast<Boss*>(object))
		{
			if (!dynamic_cast<Boss*>(object)->getAliveStatus())
			{
				for (int i = 0; i < 10; i++) {
					m_effects->addParticles("ProjectileExplosionEmitter", object->getPosition() + glm::vec3(0.0f, 2.5f, 0.0f),
						glm::vec3(Randomizer::single(-100.0f, 100.0f) / 25.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 25.0f), 0.50f);
				}

				this->m_numberOfEnemies--;
				delete m_gameObjects[i];
				m_gameObjects.erase(m_gameObjects.begin() + i);
				continue;
			}
		}
		if (dynamic_cast<PowerUps*>(object))
		{
			if (dynamic_cast<PowerUps*>(object)->powerTriggered())
			{
				delete m_gameObjects[i];
				m_gameObjects[i] = nullptr;
				m_gameObjects.erase(m_gameObjects.begin() + i);
				continue;
			}
		}
	}
}

void GameObjectManager::handleEnemyAttacks(GameObject* object, float dt)
{
	if (object->meleeRange(dt))
	{
		HitDescription desc;
		if (dynamic_cast<Walker*>(object))
		{
			Walker* walker = dynamic_cast<Walker*>(object);
			desc.walker = walker;
			m_player->hit(desc);
		}
		if (dynamic_cast<Shooter*>(object))
		{
			Shooter* shooter = dynamic_cast<Shooter*>(object);
			desc.shooter = shooter;
			m_player->hit(desc);
		}
		if (dynamic_cast<Boss*>(object))
		{
			Boss* boss = dynamic_cast<Boss*>(object);
			desc.boss = boss;
			m_player->hit(desc);
		}
	}
}

void GameObjectManager::roomManager(GameObject* object) {
	
	if (m_numberOfEnemies == 0 && m_isLocked) {
		if (m_rooms.at(m_currentRoom)->getType() == ROOM_BOSS) {
			if (m_numberOfEnemies <= 0) {
				this->m_gameFinished = true;
			}
		}
		//Remove the room from the vector of uncleared rooms
		m_rooms.erase(m_rooms.begin() + m_currentRoom);
		//Reset the player
		m_player->setPlayerState(ROAMING);
		AudioEngine::play("ClearRoom", 1.0f);
		//Find the 'door' object
		for (size_t i = 0; i < m_gameObjects.size(); i++) {
			if (m_gameObjects.at(i)->getType() == DOOR) {
				//Remove the door
				m_doorIndex = i;
				glm::vec3 objectPosition = m_gameObjects.at(m_doorIndex)->getPosition();
				m_gameObjects.at(m_doorIndex)->setPosition(glm::vec3(objectPosition.x, 100, objectPosition.z));
				m_gameObjects.at(m_doorIndex)->setCollidable(false);
			}
		}
		this->m_currentRoom = -1;
		//Unlock the room
		m_isLocked = false;
	}
	
	//When the player is not in combat
	if (m_player->getPlayerState() == ROAMING) {
		for (size_t i = 0; i < m_rooms.size(); i++) {
			if (m_rooms.at(i)->getType() == ROOM_EMPTY) {
				//If the room is of type empty then we just set the door position to something
				glm::vec3 objectPosition = m_gameObjects.at(m_doorIndex)->getPosition();
				m_gameObjects.at(m_doorIndex)->setPosition(glm::vec3(objectPosition.x, 100, objectPosition.z));
				m_gameObjects.at(m_doorIndex)->setCollidable(false);
				continue;
			}
			if (m_rooms.at(i)->intersection(m_player->getPosition()) && m_rooms.at(i) != nullptr) {
				this->m_currentRoom = i;				
				//Lock the doors
				this->m_isLocked = !m_isLocked;
				AudioEngine::play("LockRoom", 0.9f);
				//Spawn the door
				m_gameObjects.at(m_doorIndex)->setCollidable(true);

				glm::vec3 objectPosition = m_gameObjects.at(m_doorIndex)->getPosition();
				m_gameObjects.at(m_doorIndex)->setPosition(glm::vec3(objectPosition.x, 0, objectPosition.z));				
				//Swap the play state to fighting
				m_player->setPlayerState(FIGHTING);
				//Spawn enemies
				this->spawner(m_rooms.at(i));
				m_rooms.at(i)->resetMaxMinValues(); 

			}
		}
	}
}

void GameObjectManager::spawner(Room* currentRoom) {

	int spawnOffset = 5;
	float timeBeforeSpawn = 5.0f;
	float bossTimeBeforeSpawn = 8.0f;

	Mesh* enemyMesh = MeshMap::getMesh("Enemy");
	Mesh* powerUpMesh = MeshMap::getMesh("PowerUp");
	if (currentRoom->getType() == ROOM_BOSS)
	{
		
		GameObject* enemy = new Boss(enemyMesh, BOSS, currentRoom, glm::vec3(
			currentRoom->getCentrePosition().x,
			0.f,
			currentRoom->getCentrePosition().y),
			this,
			m_projectileManager, m_effects, bossTimeBeforeSpawn);
		this->addGameObject(enemy);
	}
	for (int i = 0; i < 2; i++) {
		
		int powerUpRoulette = Randomizer::single(1, 3);

		auto cell = currentRoom->getGrid()->getFreeRandomCell();

		if (powerUpRoulette == 1)
		{
			GameObject* powerUp = new PowerUps(powerUpMesh, POWERUPS, 1.f, 0.f, 0.f, false, glm::vec3(cell.x, 0.5f, cell.z), m_effects);
			this->addGameObject(powerUp);

		}
		if (powerUpRoulette == 2)
		{
			GameObject* powerUp = new PowerUps(powerUpMesh, POWERUPS, 0.f, 1.f, 0.f, true, glm::vec3(cell.x, 0.5f, cell.z), m_effects);
			this->addGameObject(powerUp);

		}
		if (powerUpRoulette == 3)
		{
			
			GameObject* powerUp = new PowerUps(powerUpMesh, POWERUPS, 0.f, 0.f, 5.f, true, glm::vec3(cell.x, 0.5f, cell.z), m_effects);
			this->addGameObject(powerUp);

		}
	}

	//Calculate ammount of Enemies
	int numMeleeEnemies = Randomizer::single(m_walkerDifficulty, m_walkerDifficulty + 2);
	int numRangedEnemies = m_shooterDifficulty;
	m_walkerDifficulty += 2;
	if (m_walkerDifficulty % 4)
	{
		m_shooterDifficulty = m_walkerDifficulty%4;
	}
	//Spawn Melee Enemies
	for (int i = 0; i < numMeleeEnemies; i++)
	{
		auto cell = currentRoom->getGrid()->getFreeRandomCell();

		GameObject* enemy = new Walker(enemyMesh, WALKER, currentRoom, glm::vec3(cell.x, 0.0f, cell.z),
			m_effects, timeBeforeSpawn);


		/*GameObject* enemy = new Walker(enemyMesh, WALKER, currentRoom, glm::vec3(
			Randomizer::single(currentRoom->getMaxMinValues().z + spawnOffset, currentRoom->getMaxMinValues().x - spawnOffset),
			0.f,
			Randomizer::single(currentRoom->getMaxMinValues().w + spawnOffset, currentRoom->getMaxMinValues().y - spawnOffset)),
			m_effects, timeBeforeSpawn);*/

		this->addGameObject(enemy);
	}
	//Spawn Ranged Enemies
	for (int i = 0; i < numRangedEnemies; i++)
	{
		auto cell = currentRoom->getGrid()->getFreeRandomCell();

		GameObject* enemy = new Shooter(enemyMesh, SHOOTER, currentRoom, glm::vec3(cell.x, 0.0f, cell.z),
			m_projectileManager, m_effects, timeBeforeSpawn);
		
		/*GameObject* enemy = new Shooter(enemyMesh, SHOOTER, currentRoom, glm::vec3(
			Randomizer::single(currentRoom->getMaxMinValues().z + spawnOffset, currentRoom->getMaxMinValues().x - spawnOffset),
			0.f,
			Randomizer::single(currentRoom->getMaxMinValues().w + spawnOffset, currentRoom->getMaxMinValues().y - spawnOffset)),
			m_projectileManager, m_effects, timeBeforeSpawn);*/

		
		this->addGameObject(enemy);
	}

}
