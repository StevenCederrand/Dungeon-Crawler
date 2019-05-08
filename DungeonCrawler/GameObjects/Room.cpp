#include "Room.h"
#include "../System/Log.h"
#include <System/Log.h>

Room::Room(Mesh * mesh, Type type, GameObject* playerObject, const glm::vec3 & position) :
	GameObject(mesh, type, position)
{
	this->setupMaxMinValues();
	this->calculateCentrePosition();
	this->m_type = type;
	this->m_playerObject = playerObject;
	this->setScale(glm::vec3(1.f, 0.5f, 1.f));
	float length = this->getMaxMinValues().y - this->getMaxMinValues().w;
	float width = this->getMaxMinValues().x - this->getMaxMinValues().z;
	m_gridsystem = new GridSystem(width, length, 1, this);
	m_gridsystem->constructCells();
}

Room::~Room()
{
	delete m_gridsystem;
}

void Room::update(float dt)
{
	m_gridsystem->update(dt);
}

GridSystem* Room::getGrid() const
{
	return m_gridsystem;
}

const glm::vec2& Room::getCentrePosition() const {
	return this->centrePosition;
}

void Room::calculateCentrePosition() {

	glm::vec2 OB = glm::vec2(this->getMaxMinValues().x, this->getMaxMinValues().y);
	glm::vec2 OC = glm::vec2(this->getMaxMinValues().z, this->getMaxMinValues().w);


	this->centrePosition = (OB + OC) * 0.5f;
}

bool Room::intersection(glm::vec3 position) {

	glm::vec4 maxMinValues = this->getMaxMinValues();
	//If the position is below the min value or above the max value
	if (position.x <= maxMinValues.z || position.x >= maxMinValues.x) {
		return false;
	}
	//If the position is below the min value or above the max value
	if (position.z <= maxMinValues.w || position.z >= maxMinValues.y) {
		return false;
	}

	return true;
}

void Room::setupMaxMinValues() {
	glm::vec4 maxMinValues = this->getMaxMinValues();
	maxMinValues.x -= 5;
	maxMinValues.y -= 7;
	maxMinValues.z += 5;
	maxMinValues.w += 7;
	this->setMaxMinValues(maxMinValues);

}
