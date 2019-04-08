#ifndef _GAMEOBJECTMANAGER_H
#define _GAMEOBJECTMANAGER_H
#include <vector>
#include "GameObject.h"

class GameObjectManager {

public:
	GameObjectManager();
	~GameObjectManager();

	void update(float dt);
	void addGameObject(GameObject* gameObject);
	void setPlayerRef(GameObject* player);

	const std::vector<GameObject*>& getGameObjects() const;

private:
	std::vector<GameObject*> m_gameObjects;
	GameObject* m_player;
};

#endif
