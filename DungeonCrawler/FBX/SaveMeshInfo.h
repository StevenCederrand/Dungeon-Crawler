#pragma once

#include "MeshData.h"

class SaveMeshInfo
{
private:
	
public:
	SaveMeshInfo();
	~SaveMeshInfo();
	void SaveControlPoints(FbxMesh* pMesh);
};