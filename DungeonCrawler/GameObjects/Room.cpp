#include "Room.h"
#include <System/Log.h>

Room::Room(Mesh * mesh, Type type, GameObject* playerObject ,const glm::vec3 & position) :
	GameObject(mesh, type, position)
{
	this->m_type = type;
	this->m_playerObject = playerObject;
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
