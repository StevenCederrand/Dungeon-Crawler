#pragma once

//Has memory leaks

#include "MeshData.h"

class SaveHierarchy
{
private:
	MeshData m_mesh;

public:
	SaveHierarchy();
	~SaveHierarchy();
	void SaveEntireHierarchy(FbxScene* lScene);
	void SaveNode(FbxNode* pNode);
	void SaveMesh(FbxNode* pNode);
	void SaveControlPoints(FbxMesh* pMesh);
	void SavePolygons(FbxMesh* pMesh);
	void SaveMeshName(FbxNode* pNode);
	void PrintChildName(FbxMesh* pMesh);
	MeshData getMeshObjects();
};