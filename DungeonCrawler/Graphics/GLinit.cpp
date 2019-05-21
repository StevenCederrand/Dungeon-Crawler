#include "GLinit.h"
#include "Vendor/stb/stb_image.h"
#include "System/Log.h"
#include "MeshMap.h"
#include "Globals/Paths.h"

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

Mesh* GLinit::createMesh(std::string name, ParserData* data)
{
	//Make a check if FBX is binary
	//Use the FBXParser class that takes in the binary custom files and generates the meshes
	if (MeshMap::MeshExistWithName(name))
		return MeshMap::getMesh(name);

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
		GLuint normalID = createTexture(data->getNormalMapName());
		mesh->setNormalID(normalID);
	}
	if (data->hasAmbientMap()) {
		GLuint ambientID = createTexture(data->getAmbientMapName());
		mesh->setAmbientID(ambientID);
	}
	mesh->setVao(vao);
	mesh->setTextureID(textureID);
	mesh->setNrOfIndices(int(data->getIndices().size()));
	mesh->setAmbientColor(data->getAmbientColor());
	mesh->setSpecularColor(data->getSpecularColor());
	mesh->setDiffuseColor(data->getDiffuseColor());
	mesh->setShininess(data->getShininess());
	mesh->setBoundingBoxMinMax(data->getMaxMinVector());
	mesh->setMaxMinValues(data->getMaxMinValues());
	
	MeshMap::addMesh(name, mesh);
	return mesh;
}

Mesh* GLinit::createMeshFBX(std::string name, FBXParserData* data)
{
	//Make a check if FBX is binary
	//Use the FBXParser class that takes in the binary custom files and generates the meshes
	if (MeshMap::MeshExistWithName(name))
		return MeshMap::getMesh(name);

	
	GLuint vao = createAndBindVAO();
	std::vector<GLuint> indices;

	for (int i = 0; i < data->getMeshHeaders()[0].vertexCount; i++)	//Without bounding box, //change from 0
		indices.emplace_back(i);

	bindIndices(indices);

	storeDataInAttributeList(0, 3, data->getVertexPos()); //WITHOUT BOUNDING BOX
	storeDataInAttributeList(1, 2, data->getUVs());
	storeDataInAttributeList(2, 3, data->getNormals());
	glBindVertexArray(NULL);

	std::string lTextureFiles;
	for (int i = 0; i < 100; i++)
	{
		if (data->getMaterialHeader().nameOfAlbedo[i] == ' ')
			i = 100;
		else
			lTextureFiles += data->getMaterialHeader().nameOfAlbedo[i];
	}
	std::string lEntireFilePath = TexturePath + lTextureFiles;

	GLuint textureID = createTexture(lEntireFilePath); //should be texture files name

	Mesh* mesh = new Mesh();

	if (data->getMaterialHeader().nameOfNormal[0] != ' ')
	{
		mesh->setHasNormalMap(1);

		std::string lNormalFiles;
		for (int i = 0; i < 100; i++)
		{
			if (data->getMaterialHeader().nameOfNormal[i] == ' ')
				i = 100;
			else
				lNormalFiles += data->getMaterialHeader().nameOfNormal[i];
		}
		std::string lEntireNormalFilePath = TexturePath + lNormalFiles;
		GLuint normalID = createTexture(lEntireNormalFilePath);
		mesh->setNormalID(normalID);
	}
	else
		mesh->setHasNormalMap(0);

	mesh->setVao(vao);
	mesh->setTextureID(textureID);
	mesh->setNrOfIndices(data->getVertexPos().size());	//WITH BOUNDING BOX
	mesh->setAmbientColor(glm::vec3(255, 0, 0));
	mesh->setSpecularColor(glm::vec3(255, 0, 0));
	mesh->setDiffuseColor(glm::vec3(255, 0, 0));
	mesh->setShininess(0.5f);

	mesh->setMaxMinValues(data->getMinMaxValuesMesh()); //THIS SHOULD BE THE MESH

	//hitbox
	mesh->setBoundingBoxMinMax(data->getMinMaxValuesHitbox()); //XYZ MAX XYZ MIN //this will be all of the hitboxes for the mesh


	MeshMap::addMesh(name, mesh); //adds the mesh to the meshmap, this is where the mesh is saved
	return mesh; //not used right now
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

GLuint GLinit::createTexture(std::string filename, bool useFilepath,  bool alpha)
{
	GLint width, height, channels;
	GLuint textureID;
	stbi_set_flip_vertically_on_load(0);

	unsigned char* textureData = nullptr;

	if (useFilepath) {
		textureData = stbi_load((TexturePath + filename).c_str(), &width, &height, &channels, 
			alpha ? STBI_rgb_alpha : STBI_rgb);
	}
	else
	{
		textureData = stbi_load(filename.c_str(), &width, &height, &channels,
			alpha ? STBI_rgb_alpha : STBI_rgb);
	}

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

	if (alpha) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);
	stbi_image_free(textureData);
	m_texIDs.emplace_back(&textureID);
	
	return textureID;
}
