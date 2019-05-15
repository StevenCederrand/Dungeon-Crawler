#ifndef _BOSS_H
#define _BOSS_H
#include <GameObjects/Room.h>
#include <AI/AStar.h>
#include <EnemyProjectile/ProjectileManager.h>
#include <Graphics/Effects.h>

class Boss : public GameObject
{
public:
	Boss(Mesh* mesh, Type type, Room* room, const glm::vec3& position, ProjectileManager* projectileManager, Effects* effects);
	~Boss();
	void update(float dt);


	bool meleeRange(float dt);
	void hit(const HitDescription& desc);
	Type getType();
	float getDamage()const;
	float getDistanceToPlayer() const;
	void amIDead();
	bool getAliveStatus()const;
	

private:
	void calculatePath(float dt, bool ignoreTimer, bool occupy);
	void moveToTarget(float dt);
	void updateCooldowns(float dt);
	void updateHoverEffect(float dt);
	void updateBehaviour(float dt);

private:

	float m_health;
	float m_speed;
	float m_damage;
	bool m_isPlayerClose;
	Type m_type;
	bool m_amIAlive;
	float m_attackCooldown;

	Effects* m_effects;
	ProjectileManager* m_projectileManager;

	float m_AStarTimer;
	Room* m_room;
	AStar* m_Astar;
	float m_hoverEffectTimer;

	std::vector<Node> m_path;

	bool m_shooting;
	float m_shootingRechargeCooldown;
	float m_currentShootingRechargeCooldown;
	int m_shootingRounds;
	float m_shootingSpeed;
	float m_shootingTimer;
	float m_projectileSpeed;
	float m_projectileDamage;
};

#endif