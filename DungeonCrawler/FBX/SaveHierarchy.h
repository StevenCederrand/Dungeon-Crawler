#pragma once

//Has memory leaks

#include "MeshData2.h"
#include "WriteCustomFile.h"

class SaveHierarchy
{
private:
	MeshData2 m_mesh; //will only be one, load one at a time, replaced
	WriteCustomFile m_file; //will only be one, never replaced
public:
	SaveHierarchy();
	~SaveHierarchy();
	void SaveEntireHierarchy(FbxScene* lScene);
	void SaveNode(FbxNode* pNode);
	void SaveStaticMesh(FbxNode* pNode);
	void SaveControlPoints(FbxMesh* pMesh);
	void SavePolygons(FbxMesh* pMesh);
	void SaveMeshName(FbxNode* pNode);
	void PrintChildName(FbxMesh* pMesh);

	MeshData2 getMesh()const;
};