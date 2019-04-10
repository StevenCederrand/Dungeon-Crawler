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
	deSpawn();
}

void Projectile::deSpawn()
{
	if (m_timer <= 0)
	{

	}
	if (m_timer > 0)
	{
		m_timer--;
	}
}

void Projectile::spawnProjectile(glm::dvec2 mousePos)
{
	Ray ray = Camera::active->getRayFromScreen(mousePos.x, mousePos.y, 1280, 720);;

	setPosition(ray.pos);

}

