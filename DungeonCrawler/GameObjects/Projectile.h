#pragma once
#ifndef _PROJECTILE_H
#define _PROJECTILE_H
#include "GameObject.h"

class Projectile : public GameObject {
public:
	Projectile(Mesh* mesh);
	void update(float dt);
	void deSpawn();

	void spawnProjectile(glm::dvec2 mousePos);
private:
	float m_speed;
	float m_timer;
};

#endif