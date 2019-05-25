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
	BOSS = 3,
	BOX = 10,
	LIGHTSPHERE = 12,
	DOOR = 13, 
	ROOM = 14,
	ROOM_EMPTY = 15, 
	ROOM_BOSS = 16, 
	GAMEOBJECT = 20,
	POWERUPS = 21,
	HEALTHPLANE = 22
};

class GameObject {
public:
	GameObject(Mesh* mesh, Type type, const glm::vec3& position = glm::vec3(0.f), float timeBeforeSpawn = 0.0f, float boundingBoxExpand = 0.0f);
	virtual ~GameObject();
	virtual void update(float dt) = 0;
	virtual void hit(const HitDescription& desc);
	virtual bool meleeRange(float dt);
	virtual Type getType();
	
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
	float lerp(float start, float end, float percent);

	const bool isSpawned() const;

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
protected:
	float m_timeBeforeSpawn;
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
