#pragma once

#include "MeshData.h"

class SaveMeshInfo
{
private:
	
public:
	SaveMeshInfo();
	~SaveMeshInfo();
	void ExtractMeshData(FbxScene* lScene);
	void SaveControlPoints(FbxMesh* pMesh);
	void TraverseNodeTree(FbxScene* lScene);
	void PrintChildName(FbxMesh* pMesh);
};