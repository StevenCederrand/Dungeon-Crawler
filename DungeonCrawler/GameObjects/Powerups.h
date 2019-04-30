#pragma once
#ifndef _POWERUPS_H
#define _POWERUPS_H
#include "GameObject.h"

class PowerUps :public GameObject
{
public:
	PowerUps(Mesh* mesh, Type type, float health, float damage, float speed);

	void update(float dt);
	void trigger();
	bool powerTriggered();

	float getHealthBoost() const;
	float getDamageBoost() const;
	float getSpeedBoost() const;
	glm::vec3 getBoost() const;
	Type getType();

private:

	float m_healthUp;
	float m_damageUp;
	float m_speedUp;

	Type m_type;

	bool m_trigger;
};

#endif