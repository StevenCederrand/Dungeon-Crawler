#include "SaveHierarchy.h"

SaveHierarchy::SaveHierarchy()
{
	
}

SaveHierarchy::~SaveHierarchy()
{

}

void SaveHierarchy::SaveEntireHierarchy(FbxScene* lScene)
{
	// Print the nodes of the scene and their attributes recursively.
	// Note that we are not printing the root node because it should not contain any attributes.
	// The root node of our box would be a shell.
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode)
	{
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			SaveNode(lRootNode->GetChild(i));
		}
	}
}

void SaveHierarchy::SaveNode(FbxNode* pNode)
{
	FbxNodeAttribute::EType nodeType = pNode->GetNodeAttributeByIndex(0)->GetAttributeType();

	switch (nodeType)
	{
	default:
		break;
	case FbxNodeAttribute::eMesh:
		SaveMesh(pNode);
		break;
	}

	// Print the node's attributes.
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		//PrintAttribute(pNode->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
		{
			printf("\n");
			SaveNode(pNode->GetChild(j));	//Deeper in the tree
		}
}

void SaveHierarchy::SaveMesh(FbxNode* pNode)
{
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();

	SaveMeshName(pNode);
	printf("\n");
	SaveControlPoints(lMesh);
}

void SaveHierarchy::SaveControlPoints(FbxMesh* pMesh)
{
	int i = 0;
	int lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	for (i = 0; i < lControlPointsCount; i++)
	{
		m_mesh.AddMeshPoint(lControlPoints[i]);

		/*
		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "Normal Vetor: ");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
					Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
			}
		}
		*/
	}
	m_mesh.CheckMesh();
	printf("\n\n");
	delete lControlPoints;
}

void SaveHierarchy::SavePolygons(FbxMesh* pMesh)
{
	pMesh->GetPolygonCount();
}

void SaveHierarchy::SaveMeshName(FbxNode* pNode)
{
	const char* nodeName = pNode->GetName();	//The node we get right now is the pCube1 which is the name of the cube in the outliner
	delete nodeName;
}

void SaveHierarchy::PrintChildName(FbxMesh* pMesh)
{
	printf("%s", pMesh->GetName());
}

MeshData SaveHierarchy::getMeshObjects()
{
	return m_mesh;
}
