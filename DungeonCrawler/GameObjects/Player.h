#ifndef PLAYER_h
#define PLAYER_h
#include "GameObject.h"
#include <string>
#include <vector>

class GLFWcursor;
class GLFWimage;

class Player : public GameObject {
public:
	Player(Mesh* mesh, Type type);
	virtual ~Player();

	void update(float dt);
	void hit(const HitDescription& desc);
	Type getType();

	void move(float dt);
	void rotatePlayer();
	void dash();
	void dashCd(float dt);
	void powerUpCd(float dt);
	void manualReload(float dt);
	void reloadCd(float dt);

	void weaponSwap();
	void shootAutomatic(float dt);
	void shootChargeShot(float dt);
	void shootProjectile(float dt);
	void chargeProjectile(float dt);
	void releaseChargedProjectile(float dt);
	void screenShake(float dt);
	void wobbleAnim(float dt);

	void setSpeed(float speed);
	void setHealth(float health);
	void setMaxHealth();
	void setDamage(float damage);
	//void iframeCountdown(float dt);
	void setPlayerState(const EntityState& playerState);
	void takeDamage(float damageRecieved);

	const EntityState& getPlayerState() const;
	float getSpeed()const;
	float getHealth()const;
	float getDamage()const;
	int getBulletCount()const;
	bool isShooting() const;

	const glm::vec3& getLookDirection() const;
	const float& getAngle() const;

	glm::vec3 shakeDirection() const;
	Spotlight* getSpotlight();
	Light* getFlash();
	glm::vec3 getPlayerPosition()const;

private:
	void spotlightHandler();
	void setupSoundVector();
private:
	float m_angle;
	Spotlight* m_spotlight;
	Light* m_flash;
	glm::vec3 m_movementDirection;
	//Player Variables
	float m_defaultSpeed;
	float m_speed;
	float m_health;
	float m_maxHealth;
	//float m_iframes;
	float m_sinTime;
	float m_sinAddTime;
	float m_sinDefaultAddTime;

	int m_pistolBullets;
	bool m_reloading;
	float m_reloadTime;
	float m_reloadTimeAdd;
	float m_damage;
	float m_chargeDamage;
	float m_automaticDamage;
	float m_unChargedDamage;

	Type m_type;

	float m_dash;
	bool m_dashCd;
	float m_dashTimer;

	// Shooting
	int m_weaponSlot;
	bool m_shooting;
	bool m_canShoot;
	bool m_spraying;
	float m_shootingCooldown;
	glm::vec3 m_lookDirection;
	bool m_chargeStance;
	float m_shakeIntensity;
	float m_chargeTimer;

	std::vector<unsigned char> m_image[7];
	unsigned int m_width;
	unsigned int m_height;
	GLFWcursor* m_cursor;
	GLFWimage* m_data;


	std::vector<std::string> m_walkSounds;
	float m_shake;
	glm::vec3 m_shakeDir;
	EntityState playerState;
	

	glm::vec3 m_boostResetters;
	float m_boostTimer;
	bool m_poweredUp;

	//Debugging Tools
	bool m_debug;
};
#endif