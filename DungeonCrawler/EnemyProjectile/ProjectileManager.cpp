#include "ProjectileManager.h"
#include <System/Log.h>
ProjectileManager::ProjectileManager()
{
	m_player = nullptr;
}

ProjectileManager::~ProjectileManager()
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		delete m_projectiles[i];
	}

	m_projectiles.clear();
}

void ProjectileManager::setPlayer(GameObject* player)
{
	m_player = dynamic_cast<Player*>(player);
}

void ProjectileManager::update(float dt)
{
	if (m_player != nullptr) {

		int vectorSize = m_projectiles.size();
		for (int i = vectorSize - 1; i >= 0; i--)
		{

			Projectile* proj = m_projectiles[i];
			proj->update(dt);

			if (proj->getAABB()->checkCollision(*m_player->getBoundingBoxes()[0]))
			{
				m_player->setHealth(m_player->getHealth() - proj->getDamage());
				delete proj;
				m_projectiles.erase(m_projectiles.begin() + i);

				LOG_TRACE("Hit player!");

				continue;
			}

			if (proj->isAtDestination())
			{
				delete proj;
				m_projectiles.erase(m_projectiles.begin() + i);
				LOG_TRACE("At final destination!");
				continue;
			}

		}
	}
}

void ProjectileManager::render()
{
}

void ProjectileManager::spawnProjectile(Projectile* projectile)
{
	m_projectiles.emplace_back(projectile);
}

const size_t ProjectileManager::getNumberOfEnemyProjectiles() const
{
	return m_projectiles.size();
}
