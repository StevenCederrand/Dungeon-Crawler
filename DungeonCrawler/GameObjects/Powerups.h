#pragma once
#ifndef _POWERUPS_H
#define _POWERUPS_H
#include "GameObject.h"
#include <Graphics/Effects.h>

class PowerUps : public GameObject
{
public:
	PowerUps(Mesh* mesh, Type type, float health, float damage, float speed, bool timed, glm::vec3 position, Effects* effect);
	virtual ~PowerUps();
	void update(float dt);
	void trigger();
	bool powerTriggered();


	bool getTimed() const;
	glm::vec3 getBoost() const;
	Type getType();

private:

	float m_healthUp;
	float m_damageUp;
	float m_speedUp;

	bool m_timed;
	bool m_trigger;

	float m_particleSpawnDelay;
	float m_particleSpawnTimer;
	float m_particleVelUp;

	Type m_type;
	Effects* m_effect;
};

#endif