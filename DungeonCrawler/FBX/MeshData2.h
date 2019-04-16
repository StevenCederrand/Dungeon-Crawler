#pragma once

#include "DisplayCommon.h"

class MeshData2
{
private:
	int m_currentControlPoint;
	FbxVector4 m_controlPointArr[100];

	int m_currentControlPointIndex;
	int m_controlPointIndexArr[100];

	int m_nrOfPolygons;
	int m_nrOfVerticesPerPolygon; //3 if triangulated

	int m_currentUVCoordinate;
	FbxVector2 m_UVCoordinates[10];


	void initiateArrays();
public:
	MeshData2();
	~MeshData2();
	void AddControlPoint(FbxVector4 controlPoint);
	void AddIndexPoint(int index);
	void AddUVCoordinate(FbxVector2 uVCoordinate);
	void CheckMesh();

	void setNrOfPolygons(int nrOfPolygons);
	void setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon);
};