#pragma once

#include "DisplayCommon.h"

class MeshData2
{
private:
	int m_vertexCount;

	int m_currentControlPoint;
	int m_currentControlPointIndex;
	float m_controlPointArrFloat[3][100];
	int m_controlPointIndexArr[100];

	int m_currentUVCoordinate;
	int m_currentUVIndex;
	FbxVector2 m_UVCoordinates[100];
	int m_UVCoordinateIndexArr[100];

	int m_currentNormalCoordinate;
	FbxVector4 m_normalCoordinateArr[100];	//change to a vector 3
	//Should normals also use an index array?

	//Will not save tangents or bitangents, they will be calculated in the shader

	bool m_collision;
	bool m_isStatic;

	int m_nrOfPolygons;
	int m_nrOfVerticesPerPolygon; //3 if triangulated

	void initiateArrays();
	
public:
	MeshData2();
	~MeshData2();
	void AddControlPoint(FbxVector4 controlPoint);
	void AddIndexPoint(int index);
	void AddUVCoordinate(FbxVector2 uVCoordinate);
	void AddUVIndex(int index);
	void AddNormalCoordinate(FbxVector4 normalCoordinate);
	void increaseVertexCount();
	void CheckMesh();

	void setNrOfPolygons(int nrOfPolygons);
	void setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon);
	void setCollision(bool collision);
	void setStaticMesh(bool staticMesh);

	bool getCollision()const;
	bool getIsStatic()const;
};