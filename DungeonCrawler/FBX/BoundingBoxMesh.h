#pragma once

#include "DisplayCommon.h"

class BoundingBoxMesh
{
private:
	char m_name[100];
	int m_vertexCount;

	int m_currentControlPoint;
	int m_currentControlPointIndex;
	float m_controlPoints[3][8];
	int m_controlPointIndexArr[36]; //can be exact cus box

	bool m_collision;
	bool m_staticMesh;

	int m_nrOfPolygons;
	int m_nrOfVerticesPerPolygon; //3 if triangulated

	void initiateArrays();
public:
	BoundingBoxMesh();
	~BoundingBoxMesh();
	void PrepareForNewMesh();
	void AddControlPoint(FbxVector4 controlPoint);
	void AddIndexPoint(int index);

	void IncreaseVertexCount();
	void CheckMesh();

	void setNrOfPolygons(int nrOfPolygons);
	void setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon);
	void setCollision(bool collision);
	void setStaticMesh(bool staticMesh);
	void setName(const char name[], int nameSize);

	bool getCollision()const;
};