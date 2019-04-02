#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::setVao(const unsigned int & vao)
{
	m_vao = vao;
}

void Mesh::setNrOfIndices(const unsigned int & nrOfIndices)
{
	m_nrOfIndices = nrOfIndices;
}

void Mesh::setTextureID(const unsigned int & textureID)
{
	m_textureID = textureID;
}

void Mesh::setSpecularColor(const glm::vec3 & specularColor)
{
	m_specularColor = specularColor;
}

void Mesh::setAmbientColor(const glm::vec3 & ambientColor)
{
	m_ambientColor = ambientColor;
}

void Mesh::setDiffuseColor(const glm::vec3 & diffuseColor)
{
	m_diffuseColor = diffuseColor;
}

void Mesh::setShininess(const float & shininess)
{
	m_shininess = shininess;
}

const unsigned int & Mesh::getVao() const
{
	return m_vao;
}

const unsigned int & Mesh::getNrOfIndices() const
{
	return m_nrOfIndices;
}

const unsigned int & Mesh::getTextureID() const
{
	return m_textureID;
}

const glm::vec3 & Mesh::getSpecularColor() const
{
	return m_specularColor;
}

const glm::vec3 & Mesh::getAmbientColor() const
{
	return m_ambientColor;
}

const glm::vec3 & Mesh::getdiffuseColor() const
{
	return m_diffuseColor;
}

const float & Mesh::getShininess() const
{
	return m_shininess;
}
