#include "ParserData.h"

ParserData::ParserData(int startCapacity)
{
	// To speed up the process a little bit
	m_indices.reserve(startCapacity);
	m_vertices.reserve(startCapacity);
	m_uvs.reserve(startCapacity);
	m_normals.reserve(startCapacity);
	m_maxMinVector.reserve(startCapacity);
}

ParserData::~ParserData()
{
}

/* Setters */
void ParserData::addIndex(GLuint index)
{
	m_indices.emplace_back(index);
}

void ParserData::addVertex(glm::vec3 vertex)
{
	m_vertices.emplace_back(vertex);
}

void ParserData::addUV(glm::vec2 uv)
{
	m_uvs.emplace_back(uv);
}

void ParserData::addNormal(glm::vec3 normal)
{
	m_normals.emplace_back(normal);
}

void ParserData::setTextureFilename(std::string filename)
{
	m_textureFilename = filename;
}

void ParserData::setNormalMapName(std::string filename) {
	m_normalMapName = filename;
}

void ParserData::setAmbientMapName(std::string filename) {
	this->m_ambientMapName = filename;
}

void ParserData::setDiffuseColor(GLfloat r, GLfloat g, GLfloat b)
{
	m_diffuseColor = glm::vec3(r, g, b);
}

void ParserData::setSpecularColor(GLfloat r, GLfloat g, GLfloat b)
{
	m_specularColor = glm::vec3(r, g, b);
}

void ParserData::setAmbientColor(GLfloat r, GLfloat g, GLfloat b)
{
	m_ambientColor = glm::vec3(r, g, b);
}

void ParserData::setShininess(GLfloat factor)
{
	m_shininess = factor;
}


void ParserData::setBoundingBox(const std::vector<glm::vec3>& maxMinVector)
{
	m_maxMinVector = maxMinVector;
}

void ParserData::setNormalMapStrength(float str)
{
	m_normalMapStrength = str;

}

/* Getters */
const std::vector<GLuint>& ParserData::getIndices() const
{
	return m_indices;
}

const std::vector<glm::vec3>& ParserData::getVertices() const
{
	return m_vertices;
}

const std::vector<glm::vec2>& ParserData::getUvs() const
{
	return m_uvs;
}

const std::vector<glm::vec3>& ParserData::getNormals() const
{
	return m_normals;
}

const std::string & ParserData::getTextureFilename() const
{
	return m_textureFilename;
}

const std::string & ParserData::getNormalMapName() const {
	return m_normalMapName;
}

const std::string & ParserData::getAmbientMapName() const
{
	return m_ambientMapName;
}

const glm::vec3 & ParserData::getDiffuseColor() const
{
	return m_diffuseColor;
}

const glm::vec3 & ParserData::getSpecularColor() const
{
	return m_specularColor;
}

const glm::vec3 & ParserData::getAmbientColor() const
{
	return m_ambientColor;
}

const GLfloat & ParserData::getShininess() const
{
	return m_shininess;
}


const std::vector<glm::vec3>& ParserData::getMaxMinVector() const
{
	return m_maxMinVector;
}

const GLfloat & ParserData::getNormalMapStrength() const
{
	return m_normalMapStrength;
}

const bool ParserData::hasNormalMap() {
	if (m_normalMapName != "") {
		return true;
	}
	return false;

}

const bool ParserData::hasAmbientMap()
{
	if (m_ambientMapName != "") {
		return true;
	}
	return false;
}
