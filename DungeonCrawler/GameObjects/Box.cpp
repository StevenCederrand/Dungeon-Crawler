#include "Box.h"


Box::Box(Mesh * mesh, Type type, const glm::vec3& position) :
	GameObject(mesh, type, position)
{
	this->m_type = type;
}

void Box::update(float dt)
{
	
}

