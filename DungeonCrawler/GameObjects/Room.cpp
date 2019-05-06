#include "Room.h"
#include "../System/Log.h"
#include "../Globals/Helper.h"
#include <System/Log.h>

Room::Room(Mesh * mesh, Type type, GameObject* playerObject, const glm::vec3 & position) :
	GameObject(mesh, type, position)
{
	this->setupMaxMinValues();

	this->m_type = type;
	this->m_playerObject = playerObject;
	this->setScale(glm::vec3(1.f, 0.5f, 1.f));
	m_gridsystem = new GridSystem(50, 70, 2, this);
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
