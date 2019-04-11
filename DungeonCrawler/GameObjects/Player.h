#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H
#include "GameObject.h"

class Player : public GameObject {
public:
	Player(Mesh* mesh);
	void update(float dt);

	void move(float dt);
	void rotatePlayer();
	void camPerspective();
	void dash();
	void dashCd();

	void shootProjectile();

	void setSpeed(float speed);
	void setHealth(float health);
	void setDamage(float damage);

	float getSpeed()const;
	float getHealth()const;
	float getDamage()const;
	bool isShooting() const;

private:
	glm::dvec2 m_mousePos;
	float m_angle;

	glm::vec3 m_movementDirection;
	//Player Variables
	float m_defaultSpeed;
	float m_speed;
	float m_health;
	float m_damage;

	float m_dash;
	bool m_dashCd;
	int m_timer;

	bool m_shooting;

	//Debugging Tools
	bool m_debug;
};


#endif