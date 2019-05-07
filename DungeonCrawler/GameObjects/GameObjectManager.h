#ifndef _GAMEOBJECTMANAGER_H
#define _GAMEOBJECTMANAGER_H
#include <vector>
#include <list>
#include "Player.h"
#include "Enemies/Walker.h"
#include "Enemies/Shooter.h"
#include "Enemies/Boss.h"
#include "Powerups.h"
#include "GameObject.h"
#include "Graphics/Effects.h"
#include "Parser/Parser.h"
#include "Room.h"
#include "HealthPlane.h"


class GameObjectManager {

public:
	GameObjectManager(Effects* effects);
	~GameObjectManager();


	void update(float dt);
	void addGameObject(GameObject* gameObject);
	void constructPlayerBroadPhaseBox();

	Player* getPlayer() const;
	const std::vector<GameObject*>& getGameObjects() const;
	std::vector<GameObject*>* getVectorPointer();

	

private:
	void handlePlayerCollisionAgainstObjects(float dt, GameObject* object, glm::vec3& newVel, bool& hasCollided);
	void handlePlayerShooting(float dt, GameObject* object, const glm::vec3& rayDir, float& rayLengthUntilCollision, GameObject* &hitGameObject);
	void handleDeadEnemies(float dt);
	void handleEnemyAttacks(GameObject* object, float dt);

	
	void roomManager(GameObject* object);
	void spawner(Room* currentRoom, int numberOfEnemies);


private:
	int m_numberOfEnemies; //This is per room
	
	std::vector<GameObject*> m_gameObjects;
	Player* m_player;
	Walker* m_walker;
	Shooter* m_shooter;
	PowerUps* m_powerup;
	Boss* m_boss;
	HealthPlane* m_healthPlane;
	AABB* m_broadPhaseBox;
	glm::vec4 m_maxMinValues; 
	Effects* m_effects; 

	bool m_isLocked;
	std::vector<Room*> m_rooms;
	int m_doorIndex;
	int m_currentRoom;

	
};

#endif
