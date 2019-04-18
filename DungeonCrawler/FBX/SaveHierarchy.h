#pragma once

//Has memory leaks

#include "StaticMesh.h"
#include "WriteCustomFile.h"

class SaveHierarchy
{
private:
	StaticMesh m_mesh; //will only be one, load one at a time, replaced
	WriteCustomFile m_file; //will only be one, never replaced

	void m_SaveControlPoints(FbxMesh* pMesh);
	void m_SaveControlPointsIndex(FbxMesh* pMesh, int i, int j);
	void m_SaveUVCoordinatesAndIndex(FbxMesh* pMesh, int k, int i, int j, int vertexCounter);
	void m_SaveNormals(FbxMesh* pMesh, int k, int vertexCounter);
public:
	SaveHierarchy();
	~SaveHierarchy();
	void SaveEntireHierarchy(FbxScene* lScene);
	void SaveNode(FbxNode* pNode);
	void SaveStaticMesh(FbxNode* pNode);
	
	void SaveMeshName(FbxNode* pNode);
	void PrintChildName(FbxMesh* pMesh);

	StaticMesh getMesh()const;
};