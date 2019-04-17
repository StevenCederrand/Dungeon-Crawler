#include "MeshData2.h"

MeshData2::MeshData2()
{
	m_vertexCount = 0;

	m_currentControlPoint = 0;
	m_currentControlPointIndex = 0;
	m_currentUVCoordinate = 0;
	m_currentUVIndex = 0;
	m_currentNormalCoordinate = 0;

	m_collision = 0;
	m_isStatic = 0;

	m_nrOfPolygons = 0;
	m_nrOfVerticesPerPolygon = 0;

	initiateArrays();
}

MeshData2::~MeshData2()
{

}

void MeshData2::initiateArrays()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 3; j++)
			m_controlPointArrFloat[j][i] = 0.f;
	}

	for (int i = 0; i < 100; i++)
	{
		m_UVCoordinates[i].Set(0, 0);
	}

	for (int i = 0; i < 100; i++)
	{
		m_controlPointIndexArr[i] = 0;
		m_UVCoordinateIndexArr[i] = 0;
	}

	for (int i = 0; i < 100; i++)
	{
		m_normalCoordinateArr[i].Set(0, 0, 0, 0);
	}
}

void MeshData2::AddControlPoint(FbxVector4 controlPoint)
{
	float lx = controlPoint.mData[0];
	float ly = controlPoint.mData[1];
	float lz = controlPoint.mData[2];

	m_controlPointArrFloat[0][m_currentControlPoint] = lx;
	m_controlPointArrFloat[1][m_currentControlPoint] = ly;
	m_controlPointArrFloat[2][m_currentControlPoint] = lz;

	m_currentControlPoint++;
}

void MeshData2::AddIndexPoint(int index)
{
	m_controlPointIndexArr[m_currentControlPointIndex] = index;
	m_currentControlPointIndex++;
}

void MeshData2::AddUVCoordinate(FbxVector2 uVCoordinate)
{
	m_UVCoordinates[m_currentUVCoordinate] = uVCoordinate;
	m_currentUVCoordinate++;
}

void MeshData2::AddUVIndex(int index)
{
	m_UVCoordinateIndexArr[m_currentUVIndex] = index;
	m_currentUVIndex++;
}

void MeshData2::AddNormalCoordinate(FbxVector4 normalCoordinate)
{
	m_normalCoordinateArr[m_currentNormalCoordinate] = normalCoordinate;
	m_currentNormalCoordinate++;
}

void MeshData2::increaseVertexCount()
{
	m_vertexCount++;
}

void MeshData2::CheckMesh()
{
	/*
	for (int i = 0; i < m_currentControlPoint; i++)
	{
		printf("Control Point %i Coordinates: %.2f %.2f %.2f\n", i,
			m_controlPointArr[i][0], m_controlPointArr[i][1], m_controlPointArr[i][2]);
	}
	printf("\n\n");

	for (int i = 0; i < 100; i++)
	{
		printf("Control Point Index %i: %i\n", i, m_controlPointIndexArr[i]);
	}
	printf("\n\n");

	for (int i = 0; i < m_currentUVCoordinate; i++)
	{
		printf("UV Coordinate %i: %.2f %.2f\n", i, m_UVCoordinates[i][0], m_UVCoordinates[i][1]);
	}
	printf("\n\n");

	for (int i = 0; i < m_currentUVIndex; i++)
	{
		printf("UV Coordinate Index %i: %i\n", i, m_UVCoordinateIndexArr[i]);
	}
	printf("\n\n");
	*/

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
					m_controlPointArrFloat[0][m_controlPointIndexArr[lCurrentVertex]],
					m_controlPointArrFloat[1][m_controlPointIndexArr[lCurrentVertex]],
					m_controlPointArrFloat[2][m_controlPointIndexArr[lCurrentVertex]]);
				//UV
				printf("UV: %.2f %.2f\n",
					m_UVCoordinates[m_UVCoordinateIndexArr[lCurrentVertex]][0],
					m_UVCoordinates[m_UVCoordinateIndexArr[lCurrentVertex]][1]);
				//Normals, doesnt use Index right now
				printf("Normal: %.2f %.2f %.2f\n",
					m_normalCoordinateArr[lCurrentVertex][0],
					m_normalCoordinateArr[lCurrentVertex][1],
					m_normalCoordinateArr[lCurrentVertex][2]);
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

	if (m_collision)
		printf("This mesh has collision");
	else
		printf("This mesh has no collision");
	printf("\n");

	if (m_isStatic)
		printf("This mesh is static");
	else
		printf("This mesh is dynamic");
	printf("\n\n");

	printf("Nr of vertices: %i\n\n", m_vertexCount);

}

void MeshData2::setNrOfPolygons(int nrOfPolygons)
{
	m_nrOfPolygons = nrOfPolygons;
}

void MeshData2::setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon)
{
	m_nrOfVerticesPerPolygon = nrOfVerticesPerPolygon;
}

void MeshData2::setCollision(bool collision)
{
	m_collision = collision;
}

void MeshData2::setStaticMesh(bool staticMesh)
{
	m_isStatic = staticMesh;
}

bool MeshData2::getCollision()const
{
	return m_collision;
}

bool MeshData2::getIsStatic()const
{
	return m_isStatic;
}