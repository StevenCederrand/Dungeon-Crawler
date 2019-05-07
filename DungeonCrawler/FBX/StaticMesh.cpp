#include "StaticMesh.h"

StaticMesh::StaticMesh()
{
	m_vertexCount = 0;
	m_collision = 0;
	m_staticMesh = 0;

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
}

void StaticMesh::PrepareForNewMesh()
{
	m_vertexCount = 0;
	m_collision = 0;
	m_staticMesh = 0;

	m_controlPoints.clear();
	m_controlPointIndexArr.clear();
	m_UVCoordinates.clear();
	m_UVCoordinateIndexArr.clear();
	m_normalCoordinateArr.clear();
	m_vertexArr.clear();

	initiateArrays();
}

void StaticMesh::MakeAllTheVertices(int lNrOfVertices)
{
	for (int i = 0; i < lNrOfVertices; i++)//For each vector
	{
		Vertex tempVertex;
		for (int j = 0; j < 3; j++)
		{
			tempVertex.position[j] = m_controlPoints[m_controlPointIndexArr[i]][j];
		}
		for (int j = 0; j < 2; j++)
		{
			tempVertex.UV[j] = m_UVCoordinates[m_UVCoordinateIndexArr[i]][j];
		}
		for (int j = 0; j < 3; j++)
		{
			tempVertex.normal[j] = m_normalCoordinateArr[i][j];
		}
		m_vertexArr.push_back(tempVertex);
		m_vertexCount++;
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

	m_controlPoints.push_back(temp);
}

void StaticMesh::AddIndexPoint(int index)
{
	m_controlPointIndexArr.push_back(index);
}

void StaticMesh::AddUVCoordinate(FbxVector2 uVCoordinate)
{
	float lx = uVCoordinate.mData[0];
	float ly = uVCoordinate.mData[1];
	
	std::vector<float> temp;
	temp.push_back(lx);
	temp.push_back(ly);

	m_UVCoordinates.push_back(temp);
}

void StaticMesh::AddUVIndex(int index)
{
	m_UVCoordinateIndexArr.push_back(index);
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

	m_normalCoordinateArr.push_back(temp);
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

void StaticMesh::setMaterialID(int materialID)
{
	m_materialID = materialID;
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

int StaticMesh::getVertexCount()const
{
	return m_vertexCount;
}

std::vector<Vertex> StaticMesh::getVertexArr()const
{
	return m_vertexArr;
}

int StaticMesh::getMaterialID()const
{
	return m_materialID;
}