#ifndef _BOSS_H
#define _BOSS_H
#include <GameObjects/Room.h>
#include <AI/AStar.h>
#include "../../Graphics/Effects.h"

class Boss : public GameObject
{
public:
	Boss(Mesh* mesh, Type type, Room* room, const glm::vec3& position, Effects* effects);
	~Boss();
	void update(float dt);


	bool meleeRange(float dt);
	void hit(const HitDescription& desc);
	Type getType();
	float getDamage()const;
	float getDistanceToPlayer() const;
	void amIDead();
	bool getAliveStatus()const;
	void attackCooldown(float dt);

private:
	void calculatePath(float dt);
	void moveToTarget(float dt);

private:

	float m_health;
	float m_speed;
	float m_damage;
	bool m_isPlayerClose;
	Type m_type;
	bool m_amIAlive;
	float m_attackCooldown;

	Effects* m_effects;

	float m_AStarTimer;
	Room* m_room;
	AStar* m_Astar;
	float m_hoverEffectTimer;

	std::vector<Node> m_path;
};

#endif