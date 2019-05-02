#include "GameObjectManager.h"
#include "System/Log.h"
#include "Box.h"
#include <vector>
#include "../Audio/AudioEngine.h"
#include "../Globals/Helper.h"
#include "../System/Input.h"

GameObjectManager::GameObjectManager(Effects* effects)
{
	m_effects = effects;
	m_broadPhaseBox = nullptr;
	m_player = nullptr;
	m_walker = nullptr;
	m_shooter = nullptr;
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

	if (Input::isKeyReleased(GLFW_KEY_SPACE)) {
		
	}
	//------ Player collision with map ------
	bool hasCollided = false;
	glm::vec3 newVel = glm::vec3(0);
	

	//------ Update player manually ------
	m_player->update(dt);
	m_player->updateModelMatrix();
	

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
		m_effects->addParticles("GunFlareEmitter", m_player->getPosition() + glm::vec3(xAngle, 0.f, zAngle) * offset, 5.f, 0.2f);
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


		if (object->getType() == ROOM) {
			this->roomManager(object);
		}

		// Update the object
		object->setPlayerPosition(m_player->getPosition());
		object->internalUpdate(dt);
		object->update(dt);
		object->updateModelMatrix();

		// Handle collisions if there is any
		handlePlayerCollisionAgainstObjects(dt, object, newVel, hasCollided);

		// If player is shooting then handle it
		if (m_player->isShooting()) {
			handlePlayerShooting(dt, object, rayDirection, rayLengthUntilCollision, objectHit);
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
				hitEnemy = true;
				objectHit->setHit();
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

			HitDescription desc;
			desc.player = m_player;
			objectHit->hit(desc);

			if (hitEnemy){
				AudioEngine::play("gun_impact_enemy", .5f);
				m_effects->addParticles("BloodEmitter", gunshotCollisionPoint, 5.f, 0.2f, 5.f);
			}
			else{
				m_effects->addParticles("WallSmokeEmitter", gunshotCollisionPoint, 5.f, 0.2f, 5.f);
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

		if (objectType == SHOOTER || objectType == WALKER) {
			this->m_numberOfEnemies++;
		}
		else if (objectType == ROOM) {
			Room* room = dynamic_cast<Room*>(gameObject);
			//this->setupMaxMinValues(room);
			this->m_rooms.push_back(room);
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
					float remainingTime = 1.0f - collisionTime;
					hasCollided = true;
					float dotprod = (newVel.x * nz + newVel.z * nx) * remainingTime;
					newVel.x = dotprod * nz;
					newVel.z = dotprod * nx;
					HitDescription desc;

					if (dynamic_cast<Walker*>(object))
					{
						m_walker = dynamic_cast<Walker*>(object);
						desc.walker = m_walker; 
						m_player->hit(desc);
					}
					if (dynamic_cast<Shooter*>(object))
					{
						m_shooter = dynamic_cast<Shooter*>(object);
						desc.shooter = m_shooter;
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
				this->m_numberOfEnemies--;
				delete m_gameObjects[i];
				m_gameObjects.erase(m_gameObjects.begin() + i);
				continue;
			}
		}
	}
}

void GameObjectManager::roomManager(GameObject* object) {
	
	if (m_numberOfEnemies == 0 && m_isLocked) {
		m_rooms.erase(m_rooms.begin() + m_currentRoom);
		m_player->setPlayerState(ROAMING);
		LOG_WARNING("CLEARED ROOM");
		for (size_t i = 0; i < m_gameObjects.size(); i++) {
			if (m_gameObjects.at(i)->getType() == DOOR) {
				m_doorIndex = i;
				glm::vec3 objectPosition = m_gameObjects.at(i)->getPosition();
				m_gameObjects.at(i)->setPosition(glm::vec3(objectPosition.x, 100, objectPosition.z));
				//Move the bounding box?
				m_gameObjects.at(i)->setCollidable(false);
				m_numberOfEnemies = 3; 
			}
		}
		m_isLocked = false;
	}
		
	//When the player is not in combat
	if (m_player->getPlayerState() == ROAMING) {
		for (size_t i = 0; i < m_rooms.size(); i++) {
			//if the player has entered an uncharted room
			if (m_rooms.at(i)->intersection(m_player->getPosition())) {
				LOG_WARNING("ENTERED ROOM");

				//Lock the doors
				this->m_isLocked = !m_isLocked;
				m_gameObjects.at(m_doorIndex)->setCollidable(true);
				glm::vec3 objectPosition = m_gameObjects.at(i)->getPosition();
				m_gameObjects.at(m_doorIndex)->setPosition(glm::vec3(objectPosition.x, 0, objectPosition.z));
				this->m_currentRoom = i;
				
				glm::vec4 maxMin = m_rooms.at(i)->getMaxMinValues();

				//Swap the play state to fighting
				m_player->setPlayerState(FIGHTING);
			}
		}
	}
}
