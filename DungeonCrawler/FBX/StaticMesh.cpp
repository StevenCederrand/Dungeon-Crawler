#include "StaticMesh.h"

StaticMesh::StaticMesh()
{
	m_vertexCount = 0;

	m_currentControlPoint = 0;
	m_currentControlPointIndex = 0;
	m_currentUVCoordinate = 0;
	m_currentUVIndex = 0;
	m_currentNormalCoordinate = 0;

	m_collision = 0;
	m_staticMesh = 0;

	m_nrOfPolygons = 0;
	m_nrOfVerticesPerPolygon = 0;

	initiateArrays();
}

StaticMesh::~StaticMesh()
{

}

void StaticMesh::initiateArrays()
{
	for (int i = 0; i < 100; i++)
	{
		m_name[i] = ' ';
	}

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 3; j++)
			m_controlPoints[j][i] = 0.f;
	}

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 2; j++)
			m_UVCoordinates[j][i] = 0.f;
	}

	for (int i = 0; i < 100; i++)
	{
		m_controlPointIndexArr[i] = 0;
		m_UVCoordinateIndexArr[i] = 0;
	}

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 3; j++)
			m_normalCoordinateArr[j][i] = 0.f;
	}
}

void StaticMesh::PrepareForNewMesh()
{
	m_vertexCount = 0;

	m_currentControlPoint = 0;
	m_currentControlPointIndex = 0;
	m_currentUVCoordinate = 0;
	m_currentUVIndex = 0;
	m_currentNormalCoordinate = 0;

	m_collision = 0;
	m_staticMesh = 0;

	m_nrOfPolygons = 0;
	m_nrOfVerticesPerPolygon = 0;

	initiateArrays();
}

void StaticMesh::AddControlPoint(FbxVector4 controlPoint)
{
	float lx = controlPoint.mData[0];
	float ly = controlPoint.mData[1];
	float lz = controlPoint.mData[2];

	m_controlPoints[0][m_currentControlPoint] = lx;
	m_controlPoints[1][m_currentControlPoint] = ly;
	m_controlPoints[2][m_currentControlPoint] = lz;

	m_currentControlPoint++;
}

void StaticMesh::AddIndexPoint(int index)
{
	m_controlPointIndexArr[m_currentControlPointIndex] = index;
	m_currentControlPointIndex++;


	//VECTOR PART
	m_controlPointIndexArrVECTOR.push_back(index);
}

void StaticMesh::AddUVCoordinate(FbxVector2 uVCoordinate)
{
	float lx = uVCoordinate.mData[0];
	float ly = uVCoordinate.mData[1];

	m_UVCoordinates[0][m_currentUVCoordinate] = lx;
	m_UVCoordinates[1][m_currentUVCoordinate] = ly;

	m_currentUVCoordinate++;
}

void StaticMesh::AddUVIndex(int index)
{
	m_UVCoordinateIndexArr[m_currentUVIndex] = index; //CORRECT
	m_currentUVIndex++;
}

void StaticMesh::AddNormalCoordinate(FbxVector4 normalCoordinate)
{
	float lx = normalCoordinate.mData[0];
	float ly = normalCoordinate.mData[1];
	float lz = normalCoordinate.mData[2];
	
	m_normalCoordinateArr[0][m_currentNormalCoordinate] = lx;
	m_normalCoordinateArr[1][m_currentNormalCoordinate] = ly;
	m_normalCoordinateArr[2][m_currentNormalCoordinate] = lz;

	m_currentNormalCoordinate++;
}

void StaticMesh::increaseVertexCount()
{
	m_vertexCount++;
}

void StaticMesh::CheckMesh()
{
	printf("Name of mesh: ");
	for (int i = 0; i < 100; i++)
	{
		printf("%c", m_name[i]);
	}
	printf("\n");
	printf("Nr of vertices: %i\n\n", m_vertexCount);

	printf("ControlPoint Indices:\n");
	for (int i = 0; i < 100; i++)
	{
		printf("%i ", m_controlPointIndexArr[i]);
	}
	printf("\n\n");

	printf("UV Indices:\n");
	for (int i = 0; i < 100; i++)
	{
		printf("%i ", m_UVCoordinateIndexArr[i]);
	}
	printf("\n\n");

	if (m_collision)
		printf("Collision = 1");
	else
		printf("Collision = 0");
	printf("\n");

	if (m_staticMesh)
		printf("Static = 1");
	else
		printf("Static = 0");
	printf("\n\n");

	//print vertices
	for (int i = 0; i < 100; i++)
	{
		printf("Control point %i: ", i);
		for (int j = 0; j < 3; j++)
			printf("%.2f ", m_controlPoints[j][i]);
		printf("\nUV %i: ", i);
		for (int j = 0; j < 2; j++)
			printf("%.2f ", m_UVCoordinates[j][i]);
		printf("\nNormal %i: ", i);
		for (int j = 0; j < 3; j++)
			printf("%.2f ", m_normalCoordinateArr[j][i]);
		printf("\n\n");
	}

	//Only works for triangulated Meshes, This writes the polygons
	/*
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
				//UV
				printf("UV: %.2f %.2f\n",
					m_UVCoordinates[0][m_UVCoordinateIndexArr[lCurrentVertex]], //Reads the index correctly, but the answer is wrong
					m_UVCoordinates[1][m_UVCoordinateIndexArr[lCurrentVertex]]);
				//Normals, doesnt use Index right now
				printf("Normal: %.2f %.2f %.2f\n",
					m_normalCoordinateArr[0][lCurrentVertex],
					m_normalCoordinateArr[1][lCurrentVertex],
					m_normalCoordinateArr[2][lCurrentVertex]);
			}
		}
		else if (m_nrOfVerticesPerPolygon == 4)
		{
			//Not used right now, use if mesh is not triangulated
			printf("ERROR: MESH NOT TRIANGULATED");
		}
		printf("\n");		
	}
	*/
}

void StaticMesh::setNrOfPolygons(int nrOfPolygons)
{
	m_nrOfPolygons = nrOfPolygons;
}

void StaticMesh::setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon)
{
	m_nrOfVerticesPerPolygon = nrOfVerticesPerPolygon;
}

void StaticMesh::setCollision(bool collision)
{
	m_collision = collision;
}

void StaticMesh::setStaticMesh(bool staticMesh)
{
	m_staticMesh = staticMesh;
}

void StaticMesh::setName(const char name[], int nameSize)
{
	for (int i = 0; i < nameSize; i++)
	{
		m_name[i] = name[i];
	}
}

char StaticMesh::getNameCharacter(int space)const
{
	return m_name[space];
}

int StaticMesh::getVertexCount() const
{
	return m_vertexCount;
}

float StaticMesh::getControlPoint(int i, int j)const
{
	return m_controlPoints[j][i];
}

int StaticMesh::getControlPointIndex(int index)const
{
	return m_controlPointIndexArr[index];
}

float StaticMesh::getUVCoordinate(int i, int j)const
{
	return m_UVCoordinates[j][i];
}

int StaticMesh::getUVIndex(int index)const
{
	return m_UVCoordinateIndexArr[index];
}

float StaticMesh::getNormal(int i, int j)const
{
	return m_normalCoordinateArr[j][i];
}

bool StaticMesh::getCollision()const
{
	return m_collision;
}

bool StaticMesh::getIsStatic()const
{
	return m_staticMesh;
}

int StaticMesh::getNrOfControlPoints()const
{
	return m_currentControlPoint;
}