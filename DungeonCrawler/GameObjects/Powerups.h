#pragma once
#ifndef _POWERUPS_H
#define _POWERUPS_H
#include "GameObject.h"

class PowerUps : public GameObject
{
public:
	PowerUps(Mesh* mesh, Type type, float health, float damage, float speed, bool timed, glm::vec3 position);

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

	Type m_type;

	bool m_trigger;
};

#endif