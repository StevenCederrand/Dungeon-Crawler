#pragma once
#ifndef _SHOOTER_H
#define _SHOOTER_H
#include "../GameObject.h"

class Shooter : public GameObject {
public:
	Shooter(Mesh* mesh, Type type);
	void update(float dt);

	bool meleeRange();
	void hit(const HitDescription& desc);
	Type getType();
	float getDamage()const;
	void amIDead();
	bool getAliveStatus()const;
	float getDistanceToPlayer()const;

private:

	float m_health;
	float m_speed;
	float m_damage;
	bool m_isPlayerClose;
	Type m_type;
	bool m_amIAlive;

};

#endif // !_SHOOTER_H