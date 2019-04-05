#include "LightSphere.h"

LightSphere::LightSphere(Mesh * mesh, const glm::vec3 & position) :
	GameObject(mesh, position)
{
	setScale(glm::vec3(0.2f));
}

LightSphere::~LightSphere()
{
}

void LightSphere::update(float dt)
{
}
