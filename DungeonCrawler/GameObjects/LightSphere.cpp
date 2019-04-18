#include "LightSphere.h"

LightSphere::LightSphere(Mesh * mesh, Type type, const glm::vec3 & position) :
	GameObject(mesh, type, position)
{
	this->m_type = type;
	setScale(glm::vec3(0.2f));
}

LightSphere::~LightSphere()
{
}

void LightSphere::update(float dt)
{
}
