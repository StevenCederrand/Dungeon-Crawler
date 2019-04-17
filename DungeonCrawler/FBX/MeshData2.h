#pragma once

#include "DisplayCommon.h"

class MeshData2
{
private:
	int m_currentControlPoint;
	int m_currentControlPointIndex;
	FbxVector4 m_controlPointArr[100];
	int m_controlPointIndexArr[100];

	int m_currentUVCoordinate;
	int m_currentUVIndex;
	FbxVector2 m_UVCoordinates[100];
	int m_UVCoordinateIndexArr[100];

	int m_currentNormalCoordinate;
	FbxVector4 m_normalCoordinateArr[100];
	//Should normals also use an index array?

	bool m_collision;
	bool m_staticMesh;

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
	void CheckMesh();

	void setNrOfPolygons(int nrOfPolygons);
	void setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon);
	void setCollision(bool collision);
	void setStaticMesh(bool staticMesh);
};