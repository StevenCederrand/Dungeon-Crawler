#pragma once

#include "DisplayCommon.h"
#include <vector>
#include "HeaderStructs.h"

class StaticMesh
{
private:
	char m_name[100];
	unsigned int m_vertexCount;
	int m_materialID;
	bool m_collision;
	bool m_staticMesh;

	std::vector<std::vector<float>> m_controlPoints;		//pos
	std::vector<int> m_controlPointIndexArr;

	std::vector<std::vector<float>> m_UVCoordinates;		//UV
	std::vector<int> m_UVCoordinateIndexArr;

	std::vector<std::vector<float>> m_normalCoordinateArr;	//normal

	std::vector<Vertex> m_vertexArr; //Full vertices with pos, uv and normal

	void initiateArrays();
public:
	StaticMesh();
	~StaticMesh();
	void PrepareForNewMesh();
	void MakeAllTheVertices(int lNrOfVertices);

	void AddControlPoint(FbxVector4 controlPoint);
	void AddIndexPoint(int index);
	void AddUVCoordinate(FbxVector2 uVCoordinate);
	void AddUVIndex(int index);
	void AddNormalCoordinate(FbxVector4 normalCoordinate);

	void setCollision(bool collision);
	void setStaticMesh(bool staticMesh);
	void setName(const char name[], int nameSize);
	void setMaterialID(int materialID);

	char getNameCharacter(int space)const;
	bool getCollision()const;
	bool getIsStatic()const;
	int getVertexCount()const;
	std::vector<Vertex> getVertexArr()const;
	int getMaterialID()const;
};

/*
MeshHeader myHeader;
vector<Vertex> Vertices;

-go through FBX file, through control points and polygons and fill in the vertices array.
- pushing each vertex into Vertices array.
---------------------- -
myHeader.vertexCount = Vertices.size();
myHeader....
write myHeader to disk.
file.write(Vertices.data(), sizeof(Vertex) * vertexCount);
*/