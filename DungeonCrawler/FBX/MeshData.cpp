#include "MeshData.h"

MeshData::MeshData()
{
	m_nrOfControlPoints = 0;
	m_currentControlPoint = 0;
	m_controlPoints[m_nrOfControlPoints];
}

MeshData::~MeshData()
{
	for (int i = 0; i < m_nrOfControlPoints; i++)
	{
		delete m_controlPoints[i];
	}
	delete m_controlPoints;
}

void MeshData::AddMeshPoint(FbxVector4 controlPoint)
{
	m_controlPoints[m_nrOfControlPoints] = controlPoint;
	m_nrOfControlPoints++;
}

void MeshData::CheckMesh()
{
	for (int i = 0; i < m_nrOfControlPoints; i++)
	{
		Display3DVector("Coordinates: ", m_controlPoints[i]);
	}
}