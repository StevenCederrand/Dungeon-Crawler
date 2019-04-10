#include "Box.h"
#include "System/Input.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Projectile.h"

Box::Box(Mesh * mesh, const glm::vec3& position) :
	GameObject(mesh, position)
{

}

void Box::update(float dt)
{

}

