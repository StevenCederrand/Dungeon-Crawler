#include "Room.h"
#include <System/Log.h>

Room::Room(Mesh * mesh, Type type,const glm::vec3 & position) :
	GameObject(mesh, type, position)
{
	this->m_type = type;
	m_gridsystem = new GridSystem(50, 70, 2, this);
	m_gridsystem->constructCells();
}

Room::~Room()
{
	delete m_gridsystem;
}

void Room::update(float dt)
{
	
}

GridSystem* Room::getGrid() const
{
	return m_gridsystem;
}
