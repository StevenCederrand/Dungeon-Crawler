#ifndef _GAMEOBJECTMANAGER_H
#define _GAMEOBJECTMANAGER_H
#include <vector>
#include "Player.h"
#include "GameObject.h"
#include "Graphics/Effects.h"

class GameObjectManager {

public:
	GameObjectManager(Effects* effects);
	~GameObjectManager();

	void update(float dt);
	void addGameObject(GameObject* gameObject);
	void constructPlayerBroadPhaseBox();

	const std::vector<GameObject*>& getGameObjects() const;

private:
	void handlePlayerCollisionAgainstObjects(float dt, GameObject* object, glm::vec3& newVel, bool& hasCollided);
	void handlePlayerShooting(float dt, GameObject* object, const glm::vec3& rayDir, float& rayLengthUntilCollision, GameObject* &hitGameObject);

private:
	std::vector<GameObject*> m_gameObjects;
	Player* m_player;
	AABB* m_broadPhaseBox;
	Effects* m_effects; 
};

#endif
