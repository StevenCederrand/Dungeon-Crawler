#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H
#include "GameObject.h"

class Player : public GameObject {
public:
	Player(Mesh* mesh);
	void update(float dt);
	void move(float dt);
	void rotatePlayer(float dt);



	void setSpeed(float speed);
	void setHealth(float health);
	void setDamage(float damage);

	float getSpeed()const;
	float getHealth()const;
	float getDamage()const;


private:
	glm::dvec2 m_mousePos;
	float m_angle;



	//Player Variables
	float m_speed;
	float m_health;
	float m_damage;

	//Debugging Tools
	bool m_debug;
};


#endif