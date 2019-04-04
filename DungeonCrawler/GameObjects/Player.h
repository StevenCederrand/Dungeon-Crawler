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

private:
	glm::dvec2 m_mousePos;
	float m_angle;
	float a, b, c, d, e;
};


#endif