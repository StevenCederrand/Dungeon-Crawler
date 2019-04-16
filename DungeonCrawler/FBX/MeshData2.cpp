#include "MeshData2.h"

MeshData2::MeshData2()
{
	
	m_currentControlPoint = 0;
	m_currentControlPointIndex = 0;
	m_nrOfPolygons = 0;
	m_nrOfVerticesPerPolygon = 3;
	m_currentUVCoordinate = 0;
	initiateArrays();
	
}

MeshData2::~MeshData2()
{
	/*
	for (int i = 0; i < m_maxNrOfControlPoints; i++)
	{
		delete m_controlPointArr[i];
	}
	delete m_controlPointArr;
	*/
}

void MeshData2::initiateArrays()
{
	for (int i = 0; i < 100; i++)
	{
		m_controlPointArr[i].Set(0, 0, 0, 0);
	}

	for (int i = 0; i < 10; i++)
	{
		m_UVCoordinates[i].Set(0, 0);
	}

	for (int i = 0; i < 100; i++)
	{
		m_controlPointIndexArr[i] = 0;
	}
}

void MeshData2::AddControlPoint(FbxVector4 controlPoint)
{
	m_controlPointArr[m_currentControlPoint] = controlPoint;
	m_currentControlPoint++;
}

void MeshData2::AddUVCoordinate(FbxVector2 uVCoordinate)
{
	m_UVCoordinates[m_currentUVCoordinate] = uVCoordinate;
	m_currentUVCoordinate++;
}

void MeshData2::AddIndexPoint(int index)
{
	m_controlPointIndexArr[m_currentControlPointIndex] = index;
	m_currentControlPointIndex++;
}

void MeshData2::CheckMesh()
{
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

	for (int i = 0; i < m_nrOfPolygons; i++)
	{
		printf("Polygon %i: ", i);
		for (int j = 0; j < m_nrOfVerticesPerPolygon; j++)
		{
			int currentPlaceInVerticeIndexArr = (m_nrOfVerticesPerPolygon * i) + j;
			//add nr of vetrices per polugon here in for
			printf("V%i: %.2f %.2f %.2f  ", j, 
				m_controlPointArr[m_controlPointIndexArr[currentPlaceInVerticeIndexArr]][0],
				m_controlPointArr[m_controlPointIndexArr[currentPlaceInVerticeIndexArr]][1],
				m_controlPointArr[m_controlPointIndexArr[currentPlaceInVerticeIndexArr]][2]);
		}
		printf("\n");
	}
	printf("\n\n");

}

void MeshData2::setNrOfPolygons(int nrOfPolygons)
{
	m_nrOfPolygons = nrOfPolygons;
}

void MeshData2::setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon)
{
	m_nrOfVerticesPerPolygon = nrOfVerticesPerPolygon;
}