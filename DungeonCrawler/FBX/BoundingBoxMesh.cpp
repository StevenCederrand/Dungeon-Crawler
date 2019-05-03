#include "BoundingBoxMesh.h"

BoundingBoxMesh::BoundingBoxMesh()
{
	m_collision = 0;
	m_staticMesh = 0;
	m_nrOfPolygons = 0; //DO A CHECK IF 12 and set 0 here
	m_nrOfVerticesPerPolygon = 0;

	initiateArrays();

	//VECTOR STUFF
	m_vertexCountVECTOR = 0;
}

BoundingBoxMesh::~BoundingBoxMesh()
{

}

void BoundingBoxMesh::PrepareForNewMesh()
{
	m_collision = 0;
	m_staticMesh = 0;
	m_nrOfPolygons = 0;
	m_nrOfVerticesPerPolygon = 0;

	initiateArrays();

	//VECTOR STUFF
	m_vertexCountVECTOR = 0;
}

void BoundingBoxMesh::MakeAllTheVertices(int lNrOfVertices)
{
	for (int i = 0; i < lNrOfVertices; i++)//For each vector
	{
		BoundingBoxVertex tempVertex;
		for (int j = 0; j < 3; j++)
		{
			tempVertex.position[j] = m_controlPointsVECTOR[m_controlPointIndexArrVECTOR[i]][j];
		}
		vertexArrVECTOR.push_back(tempVertex);
		m_vertexCountVECTOR++;
	}
}

void BoundingBoxMesh::initiateArrays()
{
	for (int i = 0; i < 100; i++)
	{
		m_name[i] = ' ';
	}
}

void BoundingBoxMesh::AddControlPoint(FbxVector4 controlPoint)
{
	float lx = controlPoint.mData[0];
	float ly = controlPoint.mData[1];
	float lz = controlPoint.mData[2];

	std::vector<float> temp;
	temp.push_back(lx);
	temp.push_back(ly);
	temp.push_back(lz);

	m_controlPointsVECTOR.push_back(temp);
}

void BoundingBoxMesh::AddIndexPoint(int index)
{
	m_controlPointIndexArrVECTOR.push_back(index);
}

void BoundingBoxMesh::setNrOfPolygons(int nrOfPolygons)
{
	m_nrOfPolygons = nrOfPolygons;
}

void BoundingBoxMesh::setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon)
{
	m_nrOfVerticesPerPolygon = nrOfVerticesPerPolygon;
}

void BoundingBoxMesh::setCollision(bool collision)
{
	m_collision = collision;
}

void BoundingBoxMesh::setStaticMesh(bool staticMesh)
{
	m_staticMesh = staticMesh;
}

void BoundingBoxMesh::setName(const char name[], int nameSize)
{
	for (int i = 0; i < nameSize; i++)
	{
		m_name[i] = name[i];
	}
}

char BoundingBoxMesh::getNameCharacter(int space)const
{
	return m_name[space];
}

bool BoundingBoxMesh::getCollision()const
{
	return m_collision;
}

bool BoundingBoxMesh::getIsStatic() const
{
	return m_staticMesh;
}

int BoundingBoxMesh::getVertexCountVECTOR()const
{
	return m_vertexCountVECTOR;
}

std::vector<BoundingBoxVertex> BoundingBoxMesh::getVertexArrVECTOR()const
{
	return vertexArrVECTOR;
}