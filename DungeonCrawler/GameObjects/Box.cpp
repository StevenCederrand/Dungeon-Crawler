#include "Box.h"
#include "System/Input.h"
#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>

Box::Box(Mesh * mesh) :
	GameObject(mesh)
{

}

void Box::update(float dt)
{
	if (Input::isKeyHeldDown(GLFW_KEY_I))
	{
		this->translate(glm::vec3(5.f, 0.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_J))
	{
		this->translate(glm::vec3(0.f, 0.f, -5.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_K))
	{
		this->translate(glm::vec3(-5.f, 0.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_L))
	{
		this->translate(glm::vec3(0.f, 0.f, 5.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_U)) //Why? Because
	{
		this->translate(glm::vec3(0.f, 5.f, 0.f) * dt);
	}
	if (Input::isKeyHeldDown(GLFW_KEY_O))
	{
		this->translate(glm::vec3(0.f, -5.f, 0.f) * dt);
	}
}
