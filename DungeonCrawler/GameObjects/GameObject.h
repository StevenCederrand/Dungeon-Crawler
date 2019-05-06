#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include <GLM/glm.hpp>
#include "Graphics/Mesh.h"
#include "Collision/AABB.h"
#include "../Globals/LightTypes.h"
#include "Collision/HitData.h"

enum EntityState {
	FIGHTING, 
	ROAMING
};

enum Type {
	PLAYER = 0,
	WALKER = 1,
	SHOOTER = 2,
	BOX = 10,
	ROOM = 11,
	LIGHTSPHERE = 12,
	DOOR = 13, 
	GAMEOBJECT = 20,
	POWERUPS = 21
};

class GameObject {
public:
	GameObject(Mesh* mesh, Type type, const glm::vec3& position = glm::vec3(0.f));
	virtual ~GameObject();

	virtual void update(float dt) = 0;
	void internalUpdate(float dt);
	void updateModelMatrix();
	void setPosition(const glm::vec3& position);
	void translate(const glm::vec3& translationVector);
	
	void setVelocity(const glm::vec3& velocity);
	void setTranslateRotation(const glm::vec3 translateRotation);
	void setCollidable(bool condition);
	void setScale(const glm::vec3& scale);
	void setRotation(const glm::vec3 rotation);
	void setPlayerPosition(const glm::vec3& position);
	void setMaxMinValues(const glm::vec4& maxMinValues);
	void setHit();
	// Rotates the game object to the desired position
	void lookAt(const glm::vec3& position);

	virtual void hit(const HitDescription & desc);
	virtual Type getType();

	std::vector<AABB*> getBoundingBoxes() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getScale() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getVelocity() const;
	const glm::vec3& getPlayerPosition() const;
	const glm::vec3& getColorTint() const;
	const glm::mat4& getModelMatrix() const;
	const bool isCollidable() const;
	const glm::vec4& getMaxMinValues() const;

	Mesh* getMesh() const;

private:

	Mesh* m_mesh;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
	glm::vec3 m_velocity;
	glm::vec3 m_colorTint;
	glm::vec3 m_playerPosition;
	glm::mat4 m_modelMatrix;

	float m_colorTintFadeDuration;
	bool m_isCollidable;
	std::vector<AABB*> m_boundingBoxes;
	glm::vec4 m_maxMinValues;

	Type m_type;
};


#endif
