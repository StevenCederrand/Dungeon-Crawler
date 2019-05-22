#ifndef _PROJECTILE_H
#define _PROJECTILE_H
#include <GLM/vec3.hpp>
#include <AI/Node.h>
#include <vector>
#include <Collision/aabb.h>
class Projectile {
public:
	Projectile(const glm::vec3& startPosition, const std::vector<Node>& path, float damage, float speed, float cellSize);
	~Projectile();

	void update(float dt);
	void resetInternalAnimTimer();
	void setInternalAnimIndex(int index);
	AABB* getAABB() const;
	
	const float getDamage() const;
	const glm::vec3& getPosition() const;
	const bool isAtDestination() const;
	const int& getInternalAnimIndex() const;
	const float& getInternalAnimTimer() const;
private:
	glm::vec3 m_position;
	std::vector<Node> m_path;
	float m_cellSize;
	bool m_isAtDestination;
	float m_damage;
	float m_speed;
	AABB* m_aabb;

	int m_internalAnimIndex;
	float m_internalAnimTimer;
};


#endif
