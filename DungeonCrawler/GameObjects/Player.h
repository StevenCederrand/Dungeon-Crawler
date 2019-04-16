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
	void dash();
	void dashCd(float dt);
	
	void weaponSwap();
	void shootAutomatic(float dt);
	void shootChargeShot(float dt);
	void shootProjectile(float dt);
	void chargeProjectile(float dt);
	void releaseChargedProjectile(float dt);
	void screenShake(float dt);

	void setSpeed(float speed);
	void setHealth(float health);
	void setDamage(float damage);

	void takeDamage(float damageRecieved);

	float getSpeed()const;
	float getHealth()const;
	float getDamage()const;
	bool isShooting() const;

	const glm::vec3& getLookDirection() const;
	glm::vec3 shakeDirection() const;

	glm::vec3 getPlayerPosition()const;

private:
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

	// Shooting
	int m_weaponSlot;
	bool m_shooting;
	bool m_canShoot;
	bool m_spraying;
	float m_shootingCooldown;
	glm::vec3 m_lookDirection;
	bool m_chargeStance;
	float m_shakeIntensity;
	int m_chargeTimer;
	
	
	int m_shake;
	glm::vec3 m_shakeDir;



	//Debugging Tools
	bool m_debug;
};


#endif