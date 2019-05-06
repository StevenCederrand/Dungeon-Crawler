#ifndef _PROJECTILEMANAGER_H
#define _PROJECTILEMANAGER_H
#include "Projectile.h"
#include <GameObjects/Player.h>

class ProjectileManager {
public:
	ProjectileManager();
	~ProjectileManager();

	void setPlayer(GameObject* player);
	void update(float dt);
	void render();

	void spawnProjectile(Projectile* projectile);

	const size_t getNumberOfEnemyProjectiles() const;

private:
	std::vector<Projectile*> m_projectiles;
	Player* m_player;
};

#endif
