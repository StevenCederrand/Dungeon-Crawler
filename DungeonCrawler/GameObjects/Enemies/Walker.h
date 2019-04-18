#pragma once
#ifndef _WALKER_H
#define _WALKER_H
#include "../GameObject.h"

class Walker : public GameObject {
public:
	Walker(Mesh* mesh, Type type);
	void update(float dt);
	void move(float dt);
	void moveToPlayer(float dt);
	void findPath(glm::vec3 startPos, glm::vec3 targetPos);
	
	void hitPlayer();
	void hit(const HitDescription& desc);
	Type getType();
	float getDamage()const;

private:
	float m_health;
	float m_speed;
	float m_damage;
	bool m_isPlayerClose;
	Type m_type;
};

#endif // !_WALKER_H
