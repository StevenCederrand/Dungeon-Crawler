#include "StaticMesh.h"

StaticMesh::StaticMesh()
{
	m_collision = 0;
	m_staticMesh = 0;

	m_nrOfPolygons = 0;
	m_nrOfVerticesPerPolygon = 0;

	initiateArrays();

	//VECTOR STUFF
	m_vertexCountVECTOR = 0;
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
}

void StaticMesh::PrepareForNewMesh()
{
	m_collision = 0;
	m_staticMesh = 0;

	m_nrOfPolygons = 0;
	m_nrOfVerticesPerPolygon = 0;

	initiateArrays();

	//VECTOR PART
	m_vertexCountVECTOR = 0;
	vertexArrVECTOR.clear();
}

void StaticMesh::MakeAllTheVertices(int lNrOfVertices)
{
	for (int i = 0; i < lNrOfVertices; i++)//For each vector
	{
		Vertex tempVertex;
		for (int j = 0; j < 3; j++)
		{
			tempVertex.position[j] = m_controlPointsVECTOR[m_controlPointIndexArrVECTOR[i]][j];
		}
		for (int j = 0; j < 2; j++)
		{
			tempVertex.UV[j] = m_UVCoordinatesVECTOR[m_UVCoordinateIndexArrVECTOR[i]][j];
		}
		for (int j = 0; j < 3; j++)
		{
			tempVertex.normal[j] = m_normalCoordinateArrVECTOR[i][j];
		}
		vertexArrVECTOR.push_back(tempVertex);
		m_vertexCountVECTOR++;
	}
}

void StaticMesh::AddControlPoint(FbxVector4 controlPoint)
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

void StaticMesh::AddIndexPoint(int index)
{
	m_controlPointIndexArrVECTOR.push_back(index);
}

void StaticMesh::AddUVCoordinate(FbxVector2 uVCoordinate)
{
	float lx = uVCoordinate.mData[0];
	float ly = uVCoordinate.mData[1];
	
	std::vector<float> temp;
	temp.push_back(lx);
	temp.push_back(ly);

	m_UVCoordinatesVECTOR.push_back(temp);
}

void StaticMesh::AddUVIndex(int index)
{
	m_UVCoordinateIndexArrVECTOR.push_back(index);
}

void StaticMesh::AddNormalCoordinate(FbxVector4 normalCoordinate)
{
	float lx = normalCoordinate.mData[0];
	float ly = normalCoordinate.mData[1];
	float lz = normalCoordinate.mData[2];

	std::vector<float> temp;
	temp.push_back(lx);
	temp.push_back(ly);
	temp.push_back(lz);

	m_normalCoordinateArrVECTOR.push_back(temp);
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

bool StaticMesh::getCollision()const
{
	return m_collision;
}

bool StaticMesh::getIsStatic()const
{
	return m_staticMesh;
}

int StaticMesh::getVertexCountVECTOR()const
{
	return m_vertexCountVECTOR;
}

std::vector<Vertex> StaticMesh::getVertexArrVECTOR()const
{
	return vertexArrVECTOR;
}