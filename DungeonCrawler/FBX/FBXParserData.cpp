#include "FBXParserData.h"

FBXParserData::FBXParserData(int vectorStartValue)
{
	//initialize 
	//mainheader
	m_mainHeader.version = ' ';
	m_mainHeader.padding1 = ' ';
	m_mainHeader.padding2 = ' ';
	m_mainHeader.padding3 = ' ';
	m_mainHeader.dynamicMeshCount = 0;
	m_mainHeader.staticMeshCount = 0;
	m_mainHeader.boundingBoxCount = 0;
	m_mainHeader.materialCount = 0;

	//m_materialheader
	for (int i = 0; i < 100; i++)
	{
		m_materialHeader.nameOfAlbedo[i] = ' ';
		m_materialHeader.nameOfNormal[i] = ' ';
	}
	m_materialHeader.materialID = 0;
	m_materialHeader.nrOfTextures = 0;
	m_materialHeader.whatShader = 0;

	//Vertex
	m_vertexHeader.position[0] = 0.f;
	m_vertexHeader.position[1] = 0.f;
	m_vertexHeader.position[2] = 0.f;
	m_vertexHeader.UV[0] = 0.f;
	m_vertexHeader.UV[1] = 0.f;
	m_vertexHeader.normal[0] = 0.f;
	m_vertexHeader.normal[1] = 0.f;
	m_vertexHeader.normal[2] = 0.f;

	//boundingboxvertexheader
	m_boundingBoxVertexHeader.position[0] = 0.f;
	m_boundingBoxVertexHeader.position[1] = 0.f;
	m_boundingBoxVertexHeader.position[2] = 0.f;

	//vertice mesh data
	m_verticePos.reserve(vectorStartValue);
	m_uvs.reserve(vectorStartValue);
	m_normals.reserve(vectorStartValue);

	m_verticePosVector.reserve(vectorStartValue);

	//vertice hitbox data
	m_verticePosHitbox.reserve(vectorStartValue);

	//mesh
	m_minMaxValuesMesh.x = 0;
	m_minMaxValuesMesh.y = 0;
	m_minMaxValuesMesh.z = 0;
	m_minMaxValuesMesh.w = 0;

	//hitbox
	m_minMaxValuesHitbox.reserve(vectorStartValue);
}

FBXParserData::~FBXParserData()
{

}

void FBXParserData::setVertexHeader(Vertex vertexHeader)
{
	this->m_vertexHeader = vertexHeader;
}

//GET FUNCTIONS
MainHeader FBXParserData::getMainHeader() const
{
	return this->m_mainHeader;
}

Material FBXParserData::getMaterialHeader()const
{
	return m_materialHeader;
}

std::vector<BoundingBoxHeader> FBXParserData::getBoundingBoxHeaders() const
{
	return m_boundingBoxHeaders;
}

Vertex FBXParserData::getVertexHeader() const
{
	return this->m_vertexHeader;
}

BoundingBoxVertex FBXParserData::getBoundingBoxVertexHeader() const
{
	return BoundingBoxVertex();
}

void FBXParserData::moveVerticePosToVector()
{
	//moves the vertexArr for the mesh to the big vector containing all the vertices for all the meshes
	m_verticePosVector.emplace_back(m_verticePos);
	m_verticePos.clear(); //cleans it so it can be filled with a new meshes info
}

//ADD FUNCTIONS
void FBXParserData::addVertexPos(glm::vec3 vertexPos)
{
	m_verticePos.emplace_back(vertexPos);
}

void FBXParserData::addUV(glm::vec2 uv)
{
	m_uvs.emplace_back(uv);
}

void FBXParserData::addNormal(glm::vec3 normal)
{
	m_normals.emplace_back(normal);
}

void FBXParserData::addVertexPosHitbox(glm::vec3 vertexPosHitbox)
{
	m_verticePosHitbox.emplace_back(vertexPosHitbox);
}

void FBXParserData::addAlbedoMapName(std::string albedoMapName)
{
	m_albedoMapName.emplace_back(albedoMapName);
}

void FBXParserData::addNormalMapName(std::string normalMapName)
{
	m_normalMapName.emplace_back(normalMapName);
}

void FBXParserData::addMinMaxValuesHitbox(glm::vec3 minMaxValuesHitbox)
{
	m_minMaxValuesHitbox.emplace_back(minMaxValuesHitbox);
}

void FBXParserData::addBoundingBoxHeader(BoundingBoxHeader boundingBoxHeader)
{
	m_boundingBoxHeaders.emplace_back(boundingBoxHeader);
}

void FBXParserData::addMeshHeader(MeshHeader meshHeader)
{
	m_meshHeaders.emplace_back(meshHeader);
}

//SET FUNCTIONS
void FBXParserData::setMinMaxValuesMesh(glm::vec4 minMaxValues) //ACTUALLY IS MINMAX
{
	m_minMaxValuesMesh = minMaxValues;
}

void FBXParserData::setMainHeader(MainHeader mainHeader)
{
	m_mainHeader = mainHeader;
}

void FBXParserData::setMaterialHeader(Material materialHeader)
{
	m_materialHeader = materialHeader;
}

//GET FUNCTIONS
std::vector<glm::vec3> FBXParserData::getVertexPos()const
{
	return m_verticePos;
}

std::vector<glm::vec2> FBXParserData::getUVs()const
{
	return m_uvs;
}

std::vector<glm::vec3> FBXParserData::getNormals()const
{
	return m_normals;
}

std::vector<std::vector<glm::vec3>> FBXParserData::getVerticePosVector()const
{
	return m_verticePosVector;
}

std::vector<glm::vec3> FBXParserData::getVertexPosHitbox()const
{
	return m_verticePosHitbox;
}

std::vector<std::string> FBXParserData::getAlbedoMapName()const
{
	return m_albedoMapName;
}

std::vector<std::string> FBXParserData::getNormalMapName()const
{
	return m_normalMapName;
}

glm::vec4 FBXParserData::getMinMaxValuesMesh()const
{
	return m_minMaxValuesMesh;
}

std::vector<glm::vec3> FBXParserData::getMinMaxValuesHitbox()const
{
	return m_minMaxValuesHitbox;
}

std::vector<MeshHeader> FBXParserData::getMeshHeaders()const
{
	return m_meshHeaders;
}