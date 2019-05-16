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

void Mesh::setNormalID(const unsigned int & normalID) {
	this->m_normalID = normalID;
}

void Mesh::setAmbientID(const unsigned int & ambientID) {
	m_hasAmbientMap = true;
	m_ambientID = ambientID;
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

void Mesh::setBoundingBoxMinMax(std::vector<glm::vec3> maxMinVector)
{
	m_maxMinVector = maxMinVector;
}

void Mesh::setHasNormalMap(bool hasNormalMap)
{
	m_hasNormalMap = hasNormalMap;

}

void Mesh::setMaxMinValues(const glm::vec4& maxMinValues) {
	this->m_maxMinValues = maxMinValues;
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

const unsigned int & Mesh::getNormalID() const {
	return this->m_normalID;
}

const unsigned int & Mesh::getAmbientID() const
{	
	return m_ambientID;
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

const std::vector<glm::vec3>& Mesh::getMaxMinVector() const
{
	return m_maxMinVector;
}

const float & Mesh::getShininess() const
{
	return m_shininess;
}

const glm::vec4& Mesh::getMaxMinValues() const {
	return this->m_maxMinValues;
}

const bool & Mesh::hasNormalMap() const
{
	return m_hasNormalMap;
}

const bool & Mesh::hasAmbientMap() const
{
	return m_hasAmbientMap;
}

