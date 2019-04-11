#include "GameObjectManager.h"
#include "System/Log.h"

GameObjectManager::GameObjectManager()
{
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
	if (m_broadPhaseBox)
		m_broadPhaseBox->setParentPosition(m_player->getPosition());

	bool hasCollided = false;
	glm::vec3 newVel = glm::vec3(0);

	m_player->update(dt);
	m_player->updateModelMatrix();
	newVel = m_player->getVelocity();

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

		// Update the object
		object->update(dt);
		object->updateModelMatrix();

		// Handle collisions if there is any
		handlePlayerCollisionAgainstMap(dt, object, newVel, hasCollided);
	}

	// Lastly we translate the player with the velocity that has been 
	// modyfied by the collision handling function
	m_player->translate(newVel);
	
}

void GameObjectManager::addGameObject(GameObject * gameObject)
{
	if (gameObject) {

		// If there is no player added then dynamic cast it and check if the object that
		// is going to be added in a few lines later is the player, if it is then construct
		// a broadphase box ( Used to make collision more efficient )
		if(!m_player) 
		{
			m_player = dynamic_cast<Player*>(gameObject);
			// If this object is a player
			if (m_player) 
			{
				constructPlayerBroadPhaseBox();
			}
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

const std::vector<GameObject*>& GameObjectManager::getGameObjects() const
{
	return m_gameObjects;
}

void GameObjectManager::handlePlayerCollisionAgainstMap(float dt, GameObject * object, glm::vec3& newVel, bool& hasCollided)
{
	
	// If the object is collidable then handle collision
	if (object->isCollidable())
	{
		AABB* playerBox = m_player->getBoundingBoxes()[0];
		const std::vector<AABB*> objectBoxes = object->getBoundingBoxes();

		for (int i = 0; i < objectBoxes.size(); i++) {
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
				}
			}
		}
	}

}
