#ifndef _SHOOTER_H
#define _SHOOTER_H
#include <GameObjects/Room.h>
#include <AI/AStar.h>
#include <EnemyProjectile/ProjectileManager.h>
#include <Graphics/Effects.h>
class Shooter : public GameObject {
public:
	Shooter(Mesh* mesh, Type type, Room* room, const glm::vec3& position, ProjectileManager* projectileManager, Effects* effects);
	~Shooter();
	void update(float dt);

	void hit(const HitDescription& desc);
	Type getType();
	float getDamage()const;
	void amIDead();
	bool getAliveStatus()const;
	float getDistanceToPlayer()const;
	void floatingAnim(float dt);

private:
	void calculatePath(float dt, bool ignoreTimer, bool occupy);
	void moveToTarget(float dt);

private:

	float m_health;
	float m_speed;
	float m_damage;
	bool m_isPlayerClose;
	Type m_type;
	bool m_amIAlive;
	float m_attackCooldown;
	float m_sinTime;
	float m_sinAddTime;

	float m_castTime;
	float m_currentCastTime;
	float m_hoverEffectTimer;
	bool m_castingSpell;
	float m_maxShootingRange;

	float m_AStarTimer;
	Room* m_room;
	AStar* m_Astar;
	std::vector<Node> m_path;
	ProjectileManager* m_projectileManager;
	Effects* m_effects;
};

#endif // !_SHOOTER_H