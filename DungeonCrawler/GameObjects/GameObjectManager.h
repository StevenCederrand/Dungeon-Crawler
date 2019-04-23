#ifndef _GAMEOBJECTMANAGER_H
#define _GAMEOBJECTMANAGER_H
#include <vector>
#include <list>
#include "Player.h"
#include "Enemies/Walker.h"
#include "Enemies/Shooter.h"
#include "GameObject.h"
#include "Graphics/Effects.h"
#include "Parser/Parser.h"

class GameObjectManager {

public:
	GameObjectManager(Effects* effects);
	~GameObjectManager();

	void update(float dt);
	void addGameObject(GameObject* gameObject);
	void constructPlayerBroadPhaseBox();

	const std::vector<GameObject*>& getGameObjects() const;
	void nodecollision(ParserData* parserData);

private:
	void handlePlayerCollisionAgainstObjects(float dt, GameObject* object, glm::vec3& newVel, bool& hasCollided);
	void handlePlayerShooting(float dt, GameObject* object, const glm::vec3& rayDir, float& rayLengthUntilCollision, GameObject* &hitGameObject);
	void handleDeadEnemies(float dt);

private:
	std::vector<GameObject*> m_gameObjects;
	Player* m_player;
	Walker* m_walker;
	Shooter* m_shooter;
	AABB* m_broadPhaseBox;
	Effects* m_effects; 
};

#endif
