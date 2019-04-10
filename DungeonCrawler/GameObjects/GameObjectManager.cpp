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
		m_broadPhaseBox->setPosition(m_player->getPosition());

	bool hasCollided = false;
	glm::vec3 newVel = glm::vec3(0);

	m_player->update(dt);
	m_player->updateModelMatrix();
	newVel = m_player->getVelocity();

	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* object = m_gameObjects[i];

		if (m_player == object)
			continue;

		object->update(dt);
		object->updateModelMatrix();

	
		// If the object is collidable then handle collision
		if (object->isCollidable())
		{
			AABB* playerBox = m_player->getBoundingBox();
			AABB* objectBox = object->getBoundingBox();
			
			if (m_broadPhaseBox->checkCollision(*objectBox)) 
			{
				float nx = 0.f, nz = 0.f;
				float collisionTime = playerBox->swepAABB(newVel, *objectBox, nx, nz);
				newVel *= std::min(collisionTime, 1.0f);
				
				if (collisionTime < 1.0f)
				{
					hasCollided = true;
					float remainingTime = 1.0f - collisionTime;
					float dotprod = (newVel.x * nz + newVel.z * nx);
					newVel.x = dotprod * nz;
					newVel.z = dotprod * nx;
				}
			}
			
		}
	
	}

	m_player->translate(newVel);
	
}

void GameObjectManager::addGameObject(GameObject * gameObject)
{
	if(gameObject)
		m_gameObjects.emplace_back(gameObject);
}

void GameObjectManager::setPlayerRef(GameObject * player)
{
	m_player = player;
	
	if(!m_broadPhaseBox)
		m_broadPhaseBox = new AABB();
	
	m_broadPhaseBox->setDimensions(
		m_player->getBoundingBox()->getDimensions().x * 2.f,
		m_player->getBoundingBox()->getDimensions().y * 2.f,
		m_player->getBoundingBox()->getDimensions().z * 2.f);
	m_broadPhaseBox->setPosition(m_player->getPosition());

}

const std::vector<GameObject*>& GameObjectManager::getGameObjects() const
{
	return m_gameObjects;
}

void GameObjectManager::handlePlayerCollision(float dt)
{
	
}
