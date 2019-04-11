#include "DisplayHierarchy.h"

DisplayHierarchy::DisplayHierarchy()
{
	ResetNumTabs();
}

DisplayHierarchy::~DisplayHierarchy()
{

}

void DisplayHierarchy::ResetNumTabs()
{
	m_numTabsNew = 0;
}

void DisplayHierarchy::DisplayEntireHierarchy(FbxScene* lScene)
{
	// Print the nodes of the scene and their attributes recursively.
	// Note that we are not printing the root node because it should not contain any attributes.
	// The root node of our box would be a shell.
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode)
	{
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			PrintNode(lRootNode->GetChild(i));
	}
}

//print node, its attributes and its children, recursively.
void DisplayHierarchy::PrintNode(FbxNode* pNode)
{
	FbxNodeAttribute::EType nodeType = pNode->GetNodeAttributeByIndex(0)->GetAttributeType();

	switch (nodeType)
	{
	default:
		break;
	case FbxNodeAttribute::eMesh:
		DisplayMesh(pNode);
		break;
	}

	// Print the node's attributes.
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		printf("\n");
		m_numTabsNew++; //Goes deeper into the nodetree
		PrintNode(pNode->GetChild(j));	//Deeper in the tree
	}

	m_numTabsNew--; //Goes out of the nodetree
}

//prints tabs dependent on variable, just for better reading of info in consol
void DisplayHierarchy::PrintTabs()
{
	for (int i = 0; i < m_numTabsNew; i++)
		printf("\t");	//\t = tab, like several spaces
}

// Return a string-based representation based on the attribute type.
FbxString DisplayHierarchy::GetAttributeTypeName(FbxNodeAttribute::EType type)
{
	switch (type) {
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

//print an attribute
void DisplayHierarchy::PrintAttribute(FbxNodeAttribute* pAttribute)
{
	if (!pAttribute) return;

	//Fbx Strings, to retrieve the character array of a FbxString, use its Buffer() method.
	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType()); //uses the big list
	FbxString attrName = pAttribute->GetName();

	//std Strings
	std::string typeNameString = typeName.Buffer();
	std::string attrNameString = attrName.Buffer();

	/*
	//Check if the strings exist
	if (typeNameString != "")	//this needs to use the std string
	{
		PrintTabs();
		printf("Type Name: %s\n", typeName.Buffer());	//this needs to use the fbx string
	}
	else
	{
		PrintTabs();
		printf("No type name\n");
	}
	if (attrNameString != "")
	{
		PrintTabs();
		printf("Attribute Name: %s\n", attrName.Buffer());
	}
	else
	{
		PrintTabs();
		printf("No attribute name\n");
	}
	*/
}

void DisplayHierarchy::DisplayMesh(FbxNode* pNode)
{
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	DisplayMeshName(pNode);
	DisplayControlPoints(lMesh);
}

void DisplayHierarchy::DisplayControlPoints(FbxMesh* pMesh)
{
	int i = 0;
	int lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	PrintTabs();
	printf("Control Points:\n");
	for (i = 0; i < lControlPointsCount; i++)
	{
		PrintTabs();
		printf("Control Point %i ", i);
		Display3DVector("Coordinates: ", lControlPoints[i]);

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
	printf("\n\n");
}

void DisplayHierarchy::DisplayMeshName(FbxNode* pNode)
{
	const char* nodeName = pNode->GetName();	//The node we get right now is the pCube1 which is the name of the cube in the outliner

	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scaling = pNode->LclScaling.Get();

	// Print the contents of the node. Need to declare spaces for variables first! Also tabs for formating
	PrintTabs();
	printf("Name: %s\n", nodeName);
	PrintTabs();
	printf("Translation: %f %f %f\n", translation[0], translation[1], translation[2]);
	PrintTabs();
	printf("Rotation: %f %f %f\n", rotation[0], rotation[1], rotation[2]);
	PrintTabs();
	printf("Scaling: %f %f %f\n", scaling[0], scaling[1], scaling[2]);
	printf("\n");
}

