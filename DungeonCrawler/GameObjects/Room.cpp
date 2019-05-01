#include "Room.h"
#include "../System/Log.h"
#include "../Globals/Helper.h"
Room::Room(Mesh * mesh, Type type,const glm::vec3 & position) :
	GameObject(mesh, type, position)
{
	this->setupMaxMinValues();

	this->m_type = type;
}

Room::~Room()
{
}

void Room::update(float dt)
{
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
	LOG_ERROR("SETIP");
	glm::vec4 maxMinValues = this->getMaxMinValues();
	maxMinValues.x -= 10;
	maxMinValues.y -= 10;
	maxMinValues.z += 10;
	maxMinValues.w += 10;
	this->setMaxMinValues(maxMinValues);

}
