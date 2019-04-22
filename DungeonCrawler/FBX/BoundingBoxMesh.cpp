#include "BoundingBoxMesh.h"

BoundingBoxMesh::BoundingBoxMesh()
{
	m_vertexCount = 8; //will always be 8 uniqe vertices
	m_collision = 0;
	m_staticMesh = 0;

	initiateArrays();
}

BoundingBoxMesh::~BoundingBoxMesh()
{

}

void BoundingBoxMesh::PrepareForNewMesh()
{
	m_vertexCount = 8;
	m_collision = 0;
	m_staticMesh = 0;

	initiateArrays();
}

void BoundingBoxMesh::CheckMesh()
{
	printf("Bounding box function working");
}

void BoundingBoxMesh::initiateArrays()
{
	for (int i = 0; i < 100; i++)
	{
		m_name[i] = ' ';
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 3; j++)
			m_controlPoints[j][i] = 0;
	}

	for (int i = 0; i < 36; i++)
	{
		m_controlPointIndexArr[i] = 0;
	}
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