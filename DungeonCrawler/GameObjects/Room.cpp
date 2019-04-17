#include "Room.h"

Room::Room(Mesh * mesh, Type type,const glm::vec3 & position) :
	GameObject(mesh, type, position)
{
	this->m_type = type;
}

Room::~Room()
{
}

void Room::update(float dt)
{
}
