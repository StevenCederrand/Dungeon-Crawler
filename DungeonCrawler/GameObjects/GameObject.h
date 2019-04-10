#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include <GLM/glm.hpp>
#include "Graphics/Mesh.h"
#include "Collision/AABB.h"

class GameObject {
public:
	GameObject(Mesh* mesh, const glm::vec3& position = glm::vec3(0.f));
	~GameObject();

	virtual void update(float dt) = 0;
	void updateModelMatrix();
	void setPosition(const glm::vec3& position);
	void translate(const glm::vec3& translationVector);
	
	void setVelocity(const glm::vec3& velocity);
	void setTranslateRotation(const glm::vec3 translateRotation);
	void setCollidable(bool condition);
	void setScale(const glm::vec3& scale);
	void setRotation(const glm::vec3 rotation);

	std::vector<AABB*> getBoundingBoxes() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getScale() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getVelocity() const;
	const glm::mat4& getModelMatrix() const;
	const bool isCollidable() const;

	Mesh* getMesh() const;

private:

	Mesh* m_mesh;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
	glm::vec3 m_velocity;
	glm::mat4 m_modelMatrix;
	bool m_isCollidable;
	std::vector<AABB*> m_boundingBoxes;
	
};


#endif
