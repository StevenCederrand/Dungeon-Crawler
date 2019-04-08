#include "GLinit.h"
#include "Vendor/stb/stb_image.h"
#include "System/Log.h"
#include "MeshMap.h"
GLinit::GLinit()
{
}

GLinit::~GLinit()
{
	for (auto &id : this->m_texIDs) {
		glDeleteTextures(1, id);
	}

	for (auto &id : this->m_vbos) {
		glDeleteBuffers(1, id);
	}

	for (auto &id : this->m_vaos) {
		glDeleteVertexArrays(1, id);
	}
}

void GLinit::createMesh(std::string name, ParserData* data)
{
	if (MeshMap::MeshExistWithName(name))
		return;

	GLuint vao = createAndBindVAO();
	bindIndices(data->getIndices());
	storeDataInAttributeList(0, 3, data->getVertices());
	storeDataInAttributeList(1, 2, data->getUvs());
	storeDataInAttributeList(2, 3, data->getNormals());
	glBindVertexArray(NULL);

	GLuint textureID = createTexture(data->getTextureFilename());

	Mesh* mesh = new Mesh();

	mesh->setHasNormalMap(data->hasNormalMap());
	
	if (data->hasNormalMap()) {
		LOG_INFO("HAS NORMALMAP");
		GLuint normalID = createTexture(data->getNormalMapName());
		mesh->setNormalID(normalID);
	}

	mesh->setVao(vao);
	mesh->setTextureID(textureID);
	mesh->setNrOfIndices(int(data->getIndices().size()));
	mesh->setAmbientColor(data->getAmbientColor());
	mesh->setSpecularColor(data->getSpecularColor());
	mesh->setDiffuseColor(data->getDiffuseColor());
	mesh->setShininess(data->getShininess());
	
	MeshMap::addMesh(name, mesh);
}

GLuint GLinit::createAndBindVAO()
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	m_vaos.emplace_back(&vao);
	return vao;
}

void GLinit::bindIndices(const std::vector<GLuint>& indices)
{
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	m_vbos.emplace_back(&ibo);
}

void GLinit::storeDataInAttributeList(const GLuint & attributeNumber, const GLuint & dataPerAttribute, const std::vector<glm::vec3>& dataVec)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * dataVec.size() * dataPerAttribute, dataVec.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, dataPerAttribute, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	m_vbos.emplace_back(&vbo);
}

void GLinit::storeDataInAttributeList(const GLuint & attributeNumber, const GLuint & dataPerAttribute, const std::vector<glm::vec2>& dataVec)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * dataVec.size() * dataPerAttribute, dataVec.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, dataPerAttribute, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	m_vbos.emplace_back(&vbo);
}

GLuint GLinit::createTexture(std::string filename)
{
	GLint width, height, channels;
	GLuint textureID;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* textureData = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb);

	if (!textureData)
	{
		LOG_ERROR("Could not find texture " + filename);
		return -1;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);
	stbi_image_free(textureData);
	m_texIDs.emplace_back(&textureID);
	
	return textureID;
}
