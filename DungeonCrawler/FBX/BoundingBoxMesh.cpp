#include "BoundingBoxMesh.h"

BoundingBoxMesh::BoundingBoxMesh()
{
	m_vertexCount = 8; //will always be 8 uniqe vertices
	m_currentControlPoint = 0;
	m_currentControlPointIndex = 0;
	m_collision = 0;
	m_staticMesh = 0;
	m_nrOfPolygons = 12;
	m_nrOfVerticesPerPolygon = 3;

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
	m_nrOfPolygons = 12;
	m_nrOfVerticesPerPolygon = 3;

	initiateArrays();
}

void BoundingBoxMesh::CheckMesh()
{
	if (m_collision)
		printf("This mesh has collision");
	else
		printf("This mesh has no collision");
	printf("\n");

	if (m_staticMesh)
		printf("This mesh is static");
	else
		printf("This mesh is dynamic");
	printf("\n\n");

	printf("Nr of vertices: %i\n\n", m_vertexCount);

	printf("Name of mesh: ");
	for (int i = 0; i < 100; i++)
	{
		printf("%c", m_name[i]);
	}
	printf("\n\n");
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

void BoundingBoxMesh::AddControlPoint(FbxVector4 controlPoint)
{
	float lx = controlPoint.mData[0];
	float ly = controlPoint.mData[1];
	float lz = controlPoint.mData[2];

	m_controlPoints[0][m_currentControlPoint] = lx;
	m_controlPoints[1][m_currentControlPoint] = ly;
	m_controlPoints[2][m_currentControlPoint] = lz;

	m_currentControlPoint++;
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

bool BoundingBoxMesh::getCollision()const
{
	return m_collision;
}