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
	MainHeader m_mainHeader;
	WriteCustomFile m_file; //will only be one, never replaced
	int m_nrOfNodes;
	int m_nrOfStaticMesh;
	int m_nrOfBoundingBox;

	void m_SaveControlPoints(FbxMesh* pMesh, bool collision);
	void m_SaveControlPointsIndex(FbxMesh* pMesh, int i, int j, bool collision);
	void m_SaveUVCoordinatesAndIndex(FbxMesh* pMesh, int k, int i, int j, int vertexCounter);
	void m_SaveNormals(FbxMesh* pMesh, int k, int vertexCounter);
	void m_calculateNrOfNodes(FbxNode* pNode);
	void m_SaveStaticMeshNode(FbxNode* pNode);
	void m_SaveStaticHitboxNode(FbxNode* pNode);
	void m_SaveStaticMesh(FbxNode* pNode, bool collision, bool staticMesh);
	void m_SaveHitboxMesh(FbxNode* pNode, bool collision, bool staticMesh);
	void m_SaveMeshName(FbxNode* pNode, bool collision);
	void m_PrintChildName(FbxMesh* pMesh);
public:
	SaveHierarchy();
	~SaveHierarchy();
	void SaveEntireHierarchy(FbxScene* lScene);
};