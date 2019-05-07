#include "HealthPlane.h"


HealthPlane::HealthPlane(Mesh* mesh, Type type) :
	GameObject(mesh, type)
{
	this->setScale(glm::vec3(1.f, 1.f, 1.f));

	setPosition(glm::vec3(0.f, 0.f, 0.f));
	this->m_type = type;
}

void HealthPlane::update(float dt)
{
	setPosition(glm::vec3(getPlayerPosition().x, 0.1f, getPlayerPosition().z));
}

Type HealthPlane::getType()
{
	return this->m_type;
}