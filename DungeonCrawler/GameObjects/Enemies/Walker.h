#pragma once
#ifndef _WALKER_H
#define _WALKER_H
#include "../GameObject.h"

class Walker : public GameObject {
public:
	Walker(Mesh* mesh);
	void update(float dt);
	void move(float dt);
	void moveToPlayer(float dt);

private:
	float m_health;
	float m_speed;
	float m_damage;
};




#endif // !_WALKER_H
