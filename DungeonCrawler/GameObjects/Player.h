#ifndef PLAYER_h
#define PLAYER_h
#include "GameObject.h"

class Player : public GameObject {
public:
	Player(Mesh* mesh);
	~Player();
	void update(float dt);

	void move(float dt);
	void rotatePlayer();
	void camPerspective();
	void dash();
	void dashCd();

	void shootProjectile();
	void screenShake();

	void setSpeed(float speed);
	void setHealth(float health);
	void setDamage(float damage);

	float getSpeed()const;
	float getHealth()const;
	float getDamage()const;
	bool isShooting() const;

	const glm::vec3& getLookDirection() const;
	glm::vec3 shakeDirection() const;
	Spotlight* getSpotlight();

private:
	void spotlightHandler();
private:
	float m_angle;
	Spotlight* m_spotlight;

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
	bool m_shooting;
	bool m_canShoot;
	float m_shootingCooldown;
	glm::vec3 m_lookDirection;
	
	
	int m_shake;
	glm::vec3 m_shakeDir;
	
	//Debugging Tools
	bool m_debug;
};


#endif