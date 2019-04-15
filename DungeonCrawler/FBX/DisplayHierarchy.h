#pragma once

#include <string>
#include "DisplayCommon.h"

class DisplayHierarchy
{
private:
	int m_numTabsNew; //Tab character ("\t") counter

public:
	DisplayHierarchy();
	~DisplayHierarchy();
	void ResetNumTabs();
	void DisplayEntireHierarchy(FbxScene* lScene);
	void PrintNode(FbxNode* pNode);
	void PrintTabs();
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
	void DisplayMesh(FbxNode* pNode);
	void DisplayControlPoints(FbxMesh* pMesh);
	void DisplayPolygons(FbxMesh* pMesh);
	void DisplayMeshName(FbxNode* pNode);
	void DisplayAttribute(FbxNode* pNode);
};