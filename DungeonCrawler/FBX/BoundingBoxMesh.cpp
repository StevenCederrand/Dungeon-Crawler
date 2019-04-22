#include "BoundingBoxMesh.h"

BoundingBoxMesh::BoundingBoxMesh()
{
	m_vertexCount = 0; //will always be 36 vertices
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
	m_vertexCount = 0;
	m_collision = 0;
	m_staticMesh = 0;
	m_nrOfPolygons = 12;
	m_nrOfVerticesPerPolygon = 3;

	initiateArrays();
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

void BoundingBoxMesh::IncreaseVertexCount()
{
	m_vertexCount++;
}

void BoundingBoxMesh::CheckMesh()
{
	printf("Name of mesh: ");
	for (int i = 0; i < 100; i++)
	{
		printf("%c", m_name[i]);
	}
	printf("\n");

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

	printf("Nr of vertices: %i\n", m_vertexCount);

	printf("\n");

	//Only works for triangulated Meshes
	for (int i = 0; i < m_nrOfPolygons; i++)
	{
		printf("Polygon %i:\n", i);
		if (m_nrOfVerticesPerPolygon == 3)
		{
			for (int j = 0; j < m_nrOfVerticesPerPolygon; j++) //Per Vertex Stuff
			{
				printf("Vertex %i:\n", j);
				//Position
				//Store currentPlaceInVerticeIndexArr if load more than 1 mesh, needs to be added for other start location
				int lCurrentVertex = (m_nrOfVerticesPerPolygon * i) + j;
				printf("Position: %.2f %.2f %.2f\n",
					m_controlPoints[0][m_controlPointIndexArr[lCurrentVertex]],
					m_controlPoints[1][m_controlPointIndexArr[lCurrentVertex]],
					m_controlPoints[2][m_controlPointIndexArr[lCurrentVertex]]);
			}
		}
		else if (m_nrOfVerticesPerPolygon == 4)
		{
			//Not used right now, use if mesh is not triangulated
			printf("ERROR: MESH NOT TRIANGULATED");
		}
		printf("\n");
	}
	printf("\n\n");
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

void BoundingBoxMesh::AddIndexPoint(int index)
{
	m_controlPointIndexArr[m_currentControlPointIndex] = index;
	m_currentControlPointIndex++;
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