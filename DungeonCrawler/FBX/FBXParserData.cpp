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

	//meshheader
	for (int i = 0; i < 100; i++)
		m_meshHeader.nameOfMesh[i] = ' ';
	m_meshHeader.vertexCount = 0;
	m_meshHeader.materialID = 0;
	m_meshHeader.collision = false;
	m_meshHeader.staticMesh = false;
	m_meshHeader.padding1 = false;
	m_meshHeader.padding2 = false;

	//m_materialheader
	for (int i = 0; i < 100; i++)
	{
		m_meshHeader.nameOfMesh[i] = ' ';

		m_materialHeader.nameOfAlbedo[i] = ' ';
		m_materialHeader.nameOfNormal[i] = ' ';

		m_boundingBoxHeader.nameOfHitbox[i] = ' ';
	}
	m_materialHeader.materialID = 0;
	m_materialHeader.nrOfTextures = 0;
	m_materialHeader.whatShader = 0;

	//boundingboxheader
	m_boundingBoxHeader.vertexCount = 0;
	m_boundingBoxHeader.collision = false;
	m_boundingBoxHeader.staticMesh = false;
	m_boundingBoxHeader.padding1 = false;
	m_boundingBoxHeader.padding2 = false;

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

	//vectors
	m_verticePos.reserve(vectorStartValue);
	m_uvs.reserve(vectorStartValue);
	m_normals.reserve(vectorStartValue);

	//hitbox
	m_maxMinValues.x = 0;
	m_maxMinValues.y = 0;
	m_maxMinValues.z = 0;
	m_maxMinValues.w = 0;
}

FBXParserData::~FBXParserData()
{

}

void FBXParserData::setMainHeader(MainHeader mainHeader)
{
	this->m_mainHeader = mainHeader;
}

void FBXParserData::setMeshHeader(MeshHeader meshHeader)
{
	this->m_meshHeader = meshHeader;
}

void FBXParserData::setVertexHeader(Vertex vertexHeader)
{
	this->m_vertexHeader = vertexHeader;
}

//--------------------------------------------------------------------------------------

void FBXParserData::setMainHeaderVersion(char versionNr)
{
	this->m_mainHeader.version = versionNr;
}

void FBXParserData::setMainHeaderDynamicMeshCount(unsigned int dynamicMeshCount)
{
	this->m_mainHeader.dynamicMeshCount = dynamicMeshCount;
}

void FBXParserData::setMainHeaderStaticMeshCount(unsigned int staticMeshCount)
{
	this->m_mainHeader.staticMeshCount = staticMeshCount;
}

void FBXParserData::setMainHeaderBoundBoxCount(unsigned int nrOfBoundingBoxMeshes)
{
	this->m_mainHeader.boundingBoxCount = nrOfBoundingBoxMeshes;
}

void FBXParserData::setMainHeaderMaterialCount(unsigned int nrOfMaterials)
{
	m_mainHeader.materialCount = nrOfMaterials;
}

void FBXParserData::setMeshHeaderNameOfMesh(std::vector<char> nameOfMeshVector)
{
	for (int i = 0; i < 100; i++)
	{
		m_meshHeader.nameOfMesh[i] = nameOfMeshVector[i];
	}
	
	//nameOfMeshVector
}

void FBXParserData::setMeshHeaderVertexCountOfMesh(unsigned int vertexCount)
{
	this->m_meshHeader.vertexCount = vertexCount;
}

void FBXParserData::setMeshHeaderMaterialID(int materialID)
{
	m_meshHeader.materialID = materialID;
}

void FBXParserData::setMeshHeaderCollisionOfMesh(bool collision)
{
	this->m_meshHeader.collision = collision;
}

void FBXParserData::setMeshHeaderStaticMesh(bool staticMesh)
{
	this->m_meshHeader.staticMesh = staticMesh;
}

void FBXParserData::setBBHeaderNameOfBoundingBoxMesh(char nameOfBoundingBox[])
{
	//this->m_boundingBoxMeshHeader.nameOfHitbox = nameOfBoundingBox;
}

void FBXParserData::setBBHeaderVertexCountOfBoundingBoxMesh(unsigned int vertexCount)
{
	this->m_boundingBoxHeader.vertexCount = vertexCount;
}

void FBXParserData::setBBHeaderCollisionOfBoundingBoxMesh(bool collision)
{
	this->m_boundingBoxHeader.collision = collision;
}

void FBXParserData::setBBHeaderStaticBoundingBoxMesh(bool staticBoundingBoxMesh)
{
	this->m_boundingBoxHeader.staticMesh = staticBoundingBoxMesh;
}

void FBXParserData::setMVHeaderPositionVertexOfMesh(float positionVertex[])
{
	//this->m_vertexHeader.position = positionVertex;
}

void FBXParserData::setMVHeaderUVOfMesh(float UV[])
{
	//this->m_vertexHeader.UV = UV;
}

void FBXParserData::setMVHeaderNormalOfMesh(float normal[])
{
	//this->m_vertexHeader.normal = normal;
}

//Material
void FBXParserData::setMaterialHeaderNameOfAlbedo(std::vector<char> nameOfAlbedo)
{
	for (int i = 0; i < 100; i++)
	{
		m_materialHeader.nameOfAlbedo[i] = nameOfAlbedo[i];
	}
}

void FBXParserData::setMaterialHeaderNameOfNormal(std::vector<char> nameOfNormal)
{
	for (int i = 0; i < 100; i++)
	{
		m_materialHeader.nameOfNormal[i] = nameOfNormal[i];
	}
}

void FBXParserData::setMaterialHeaderMaterialID(unsigned int materialID)
{
	m_materialHeader.materialID = materialID;
}

void FBXParserData::setMaterialHeaderNrOfTextures(unsigned int nrOfTextures)
{
	m_materialHeader.nrOfTextures = nrOfTextures;
}

void FBXParserData::setMaterialHeaderWhatShader(unsigned int whatShader)
{
	m_materialHeader.whatShader = whatShader;
}

//bouding box
void FBXParserData::setBBVHeaderPositionOfBoundingBox(float position[])
{
	//this->m_boundingBoxVertexHeader.position = position;
}

//--------------------------------------------------------------------------------------





MainHeader FBXParserData::getMainHeader() const
{
	return this->m_mainHeader;
}

MeshHeader FBXParserData::getMeshHeader() const
{
	return this->m_meshHeader;
}

Material FBXParserData::getMaterialHeader()const
{
	return m_materialHeader;
}

BoundingBoxHeader FBXParserData::getBoundingBoxHeader() const
{
	return m_boundingBoxHeader;
}

Vertex FBXParserData::getVertexHeader() const
{
	return this->m_vertexHeader;
}

BoundingBoxVertex FBXParserData::getBoundingBoxVertexHeader() const
{
	return BoundingBoxVertex();
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

void FBXParserData::addAlbedoMapName(std::string albedoMapName)
{
	m_albedoMapName.emplace_back(albedoMapName);
}

void FBXParserData::addNormalMapName(std::string normalMapName)
{
	m_normalMapName.emplace_back(normalMapName);
}

//SET FUNCTIONS
void FBXParserData::setMaxMinValues(glm::vec4 maxMinValues)
{
	m_maxMinValues = maxMinValues;
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

std::vector<std::string> FBXParserData::getAlbedoMapName()const
{
	return m_albedoMapName;
}

std::vector<std::string> FBXParserData::getNormalMapName()const
{
	return m_normalMapName;
}

glm::vec4 FBXParserData::getMaxMinValues()const
{
	return m_maxMinValues;
}