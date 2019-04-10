#include "Projectile.h"
#include "System/Log.h"
#include "Utility/Camera.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

Projectile::Projectile(Mesh * mesh) :
	GameObject(mesh)
{

}

void Projectile::update(float dt)
{
	
}

void Projectile::spawnProjectile(glm::dvec2 mousePos)
{
	//glfwGetCursorPos(glfwGetCurrentContext(), &mousePos.x, &mousePos.y);
	Ray ray = Camera::active->getRayFromScreen(mousePos.x, mousePos.y, 1280, 720);;

}
