#ifndef _DISPLAY_HIERARCHY_H
#define _DISPLAY_HIERARCHY_H

#include <fbxsdk.h>
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
	void PrintAttribute(FbxNodeAttribute* pAttribute);
	void DisplayMesh(FbxNode* pNode);
	void DisplayControlPoints(FbxMesh* pMesh);
	void DisplayMeshName(FbxNode* pNode);
};








#endif // ! _DISPLAY_HIERARCHY_H
