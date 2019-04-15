#pragma once

#include "MeshData.h"

class SaveHierarchy
{
private:
	MeshData meshBois;

public:
	SaveHierarchy();
	~SaveHierarchy();
	void SaveEntireHierarchy(FbxScene* lScene);
	void SaveNode(FbxNode* pNode);
	void SaveMesh(FbxNode* pNode);
	void SaveControlPoints(FbxMesh* pMesh);
	void SaveMeshName(FbxNode* pNode);
	void PrintChildName(FbxMesh* pMesh);
};