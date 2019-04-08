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
}

void GameObjectManager::update(float dt)
{
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* object = m_gameObjects[i];
		object->update(dt);
		object->updateModelMatrix();

		if (object != m_player && object->isCollidable())
		{
			AABB* playerBox = m_player->getBoundingBox();
			AABB* objectBox = object->getBoundingBox();

			if (playerBox->checkCollision(*objectBox))
			{
				LOG_INFO("Collision");
			}
		}
		


	} 
}

void GameObjectManager::addGameObject(GameObject * gameObject)
{
	if(gameObject)
		m_gameObjects.emplace_back(gameObject);
}

void GameObjectManager::setPlayerRef(GameObject * player)
{
	m_player = player;
}

const std::vector<GameObject*>& GameObjectManager::getGameObjects() const
{
	return m_gameObjects;
}
