#pragma once
#ifndef _POWERUPS_H
#define _POWERUPS_H
#include "GameObject.h"

class Powerups :public GameObject
{
public:
	Powerups(Mesh* mesh, Type type, float health, float damage, float speed);

	void update(float dt);
	void trigger();
	bool powerTrigger();
private:

	float m_healthUp;
	float m_damageUp;
	float m_speedUp;

	Type m_type;

	bool m_trigger;
};

#endif