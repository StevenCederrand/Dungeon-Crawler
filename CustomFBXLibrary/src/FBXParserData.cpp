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
	this->m_vertexHeader = m_vertexHeader;
}
//---------------------
void FBXParserData::setMainHeaderStaticMeshCount(int staticMeshCount)
{
	this->m_mainHeader.staticMeshCount = staticMeshCount;
}

MainHeader FBXParserData::getMainHeader() const
{
	return this->m_mainHeader;
}

MeshHeader FBXParserData::getMeshHeader() const
{
	return this->m_meshHeader;
}

Vertex FBXParserData::getVertexHeader() const
{
	return this->m_vertexHeader;
}

int FBXParserData::getMainHeaderStaticMeshCount() const
{
	return this->m_mainHeader.staticMeshCount;
}
