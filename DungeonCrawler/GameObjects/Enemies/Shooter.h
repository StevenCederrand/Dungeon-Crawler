#pragma once
#ifndef _SHOOTER_H
#define _SHOOTER_H
#include <GameObjects/Room.h>
#include <AI/AStar.h>
#include <EnemyProjectile/ProjectileManager.h>

class Shooter : public GameObject {
public:
	Shooter(Mesh* mesh, Type type, Room* room, const glm::vec3& position, ProjectileManager* projectileManager);
	~Shooter();
	void update(float dt);


	void hit(const HitDescription& desc);
	Type getType();
	float getDamage()const;
	void amIDead();
	bool getAliveStatus()const;

private:
	void calculatePath(float dt, bool ignoreTimer);
	void moveToTarget(float dt);

private:

	float m_health;
	float m_speed;
	float m_damage;
	bool m_isPlayerClose;
	Type m_type;
	bool m_amIAlive;

	float m_castTime;
	float m_currentCastTime;
	bool m_castingSpell;
	float m_maxShootingRange;

	float m_AStarTimer;
	Room* m_room;
	AStar* m_Astar;
	std::vector<Node> m_path;
	ProjectileManager* m_projectileManager;

};

#endif // !_SHOOTER_H