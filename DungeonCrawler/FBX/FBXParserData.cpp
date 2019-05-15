#include "FBXParserData.h"

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
		m_meshHeader.nameOfMesh[i] = nameOfAlbedo[i];
	}
}

void FBXParserData::setMaterialHeaderNameOfNormal(std::vector<char> nameOfNormal)
{
	for (int i = 0; i < 100; i++)
	{
		m_meshHeader.nameOfMesh[i] = nameOfNormal[i];
	}
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
