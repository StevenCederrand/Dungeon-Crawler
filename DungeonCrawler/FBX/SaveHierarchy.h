#pragma once

//Has memory leaks

#include "StaticMesh.h"
#include "BoundingBoxMesh.h"
#include "WriteCustomFile.h"

class SaveHierarchy
{
private:
	StaticMesh m_staticMesh; //will only be one, load one at a time, replaced
	BoundingBoxMesh m_bBMesh;
	WriteCustomFile m_file; //will only be one, never replaced

	void m_SaveControlPoints(FbxMesh* pMesh);
	void m_SaveControlPointsIndex(FbxMesh* pMesh, int i, int j);
	void m_SaveUVCoordinatesAndIndex(FbxMesh* pMesh, int k, int i, int j, int vertexCounter);
	void m_SaveNormals(FbxMesh* pMesh, int k, int vertexCounter);
	void m_SaveNode(FbxNode* pNode);
	void m_SaveStaticMesh(FbxNode* pNode, bool collision, bool staticMesh);
	void m_SaveHitboxMesh(FbxNode* pNode);
	void m_SaveMeshName(FbxNode* pNode);
	void m_PrintChildName(FbxMesh* pMesh);
public:
	SaveHierarchy();
	~SaveHierarchy();
	void SaveEntireHierarchy(FbxScene* lScene);
};