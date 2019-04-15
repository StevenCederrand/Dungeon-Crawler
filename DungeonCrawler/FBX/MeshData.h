#pragma once

#include "DisplayCommon.h"

class MeshData
{
private:
	int m_nrOfControlPoints;
	int m_currentControlPoint;
	//FbxVector4* m_controlPoints;
public:
	MeshData();
	~MeshData();
	void AddMeshPoint(FbxVector4 controlPoint);
	void CheckMesh();
};