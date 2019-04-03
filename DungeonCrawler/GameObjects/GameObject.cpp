#include "GameObject.h"
#include <GLM/gtx/transform.hpp>

GameObject::GameObject(Mesh * mesh, const glm::vec3 & position)
{
	m_mesh = mesh;
	m_position = position;
	m_rotation = glm::vec3(0.f);
	m_scale = glm::vec3(1.f);
	updateModelMatrix();
}

GameObject::~GameObject()
{

}

void GameObject::updateModelMatrix()
{
	m_modelMatrix = glm::mat4(1.f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}

void GameObject::setPosition(const glm::vec3 & position)
{
	m_position = position;
}

void GameObject::translate(const glm::vec3 & translationVector)
{
	m_position += translationVector;
}

void GameObject::setScale(const glm::vec3 & scale)
{
	m_scale = scale;
}

void GameObject::setRotation(const glm::vec3 rotation)
{
	m_rotation = rotation;
}

const glm::vec3 & GameObject::getPosition() const
{
	return m_position;
}

const glm::vec3 & GameObject::getScale() const
{
	return m_scale;
}

const glm::vec3 & GameObject::getRotation() const
{
	return m_rotation;
}

const glm::mat4 & GameObject::getModelMatrix() const
{
	return m_modelMatrix;
}

Mesh * GameObject::getMesh() const
{
	return m_mesh;
}
