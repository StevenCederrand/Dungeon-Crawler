#pragma once

#include "DisplayCommon.h"

class BoundingBoxMesh
{
private:
	char m_name[100];
	int m_vertexCount;
	float m_controlPoints[3][8];
	int m_controlPointIndexArr[36]; //can be exact cus box

	bool m_collision;
	bool m_staticMesh;

	void initiateArrays();
public:
	BoundingBoxMesh();
	~BoundingBoxMesh();
	void PrepareForNewMesh();
	void CheckMesh();

	void setCollision(bool collision);
	void setStaticMesh(bool staticMesh);
	void setName(const char name[], int nameSize);
};