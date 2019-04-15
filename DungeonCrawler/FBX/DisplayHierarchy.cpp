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
		//PrintAttribute(pNode->GetNodeAttributeByIndex(i));

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

void DisplayHierarchy::DisplayMesh(FbxNode* pNode)
{
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	DisplayAttribute(pNode);
	DisplayMeshName(pNode);
	printf("\n");
	DisplayControlPoints(lMesh);
	DisplayPolygons(lMesh);
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

void DisplayHierarchy::DisplayPolygons(FbxMesh* pMesh) //polygons = 4 control points
{
	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	DisplayString("    Polygons");

	int vertexId = 0;
	for (i = 0; i < lPolygonCount; i++)
	{
		DisplayInt("        Polygon ", i);
		int l;

		for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
		{
			FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
			switch (lePolgrp->GetMappingMode())
			{
			case FbxGeometryElement::eByPolygon:
				if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
				{
					FBXSDK_sprintf(header, 100, "        Assigned to group: ");
					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
					DisplayInt(header, polyGroupId);
					break;
				}
			default:
				// any other mapping modes don't make sense
				DisplayString("        \"unsupported group assignment\"");
				break;
			}
		}

		int lPolygonSize = pMesh->GetPolygonSize(i);

		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			if (lControlPointIndex < 0)
			{
				DisplayString("            Coordinates: Invalid index found!");
				continue;
			}
			else
				Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);

			for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
			{
				FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);
				FBXSDK_sprintf(header, 100, "            Color vertex: ");

				switch (leVtxc->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(vertexId);
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
				FBXSDK_sprintf(header, 100, "            Texture UV: ");

				switch (leUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						Display2DVector(header, leUV->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
				FBXSDK_sprintf(header, 100, "            Normal: ");

				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);
				FBXSDK_sprintf(header, 100, "            Tangent: ");

				if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			{

				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

				FBXSDK_sprintf(header, 100, "            Binormal: ");
				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leBinormal->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			vertexId++;
		} // for polygonSize
	} // for polygonCount


	//check visibility for the edges of the mesh
	for (int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
	{
		FbxGeometryElementVisibility* leVisibility = pMesh->GetElementVisibility(l);
		FBXSDK_sprintf(header, 100, "    Edge Visibility : ");
		DisplayString(header);
		switch (leVisibility->GetMappingMode())
		{
		default:
			break;
			//should be eByEdge
		case FbxGeometryElement::eByEdge:
			//should be eDirect
			for (j = 0; j != pMesh->GetMeshEdgeCount(); ++j)
			{
				DisplayInt("        Edge ", j);
				DisplayBool("              Edge visibility: ", leVisibility->GetDirectArray().GetAt(j));
			}

			break;
		}
	}
	DisplayString("");
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
}

void DisplayHierarchy::DisplayAttribute(FbxNode* pNode)
{
	if (!pNode->GetNodeAttribute()->GetAttributeType())
	{
		printf("No type name\n");
		return;
	}
	//Fbx Strings, to retrieve the character array of a FbxString, use its Buffer() method.
	FbxString typeName = GetAttributeTypeName(pNode->GetNodeAttribute()->GetAttributeType()); //uses the big list
	std::string typeNameString = typeName.Buffer();

	PrintTabs();
	printf("Type: %s\n", typeName.Buffer());	//this needs to use the fbx string
}

