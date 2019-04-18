#pragma once

#include "DisplayCommon.h"

class BoundingBoxMesh
{
private:
	int m_vertexCount;
	float m_controlPoints[3][8];

	bool m_collision;
	bool m_isStatic;

	void initiateArrays();
public:
	BoundingBoxMesh();
};