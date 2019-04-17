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

	//Check all if they are static meshes first, put them in the file first.
	//Then hitboxes
	//then dynamic meshes
	//then material

	switch (nodeType)
	{
	default:
		break;
	case FbxNodeAttribute::eMesh:	//if its a mesh
		SaveMesh(pNode);
		break;
	}

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
	SavePolygons(lMesh);

	m_mesh.CheckMesh();
}

void SaveHierarchy::SaveControlPoints(FbxMesh* pMesh)
{
	int i = 0;
	int lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	for (i = 0; i < lControlPointsCount; i++)
	{
		m_mesh.AddControlPoint(lControlPoints[i]);

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

void SaveHierarchy::SavePolygons(FbxMesh* pMesh) //polygon = 4 vertices, Not control points!
{
	//How many polygons there are in the current mesh
	int lPolygonCount = pMesh->GetPolygonCount();
	m_mesh.setNrOfPolygons(lPolygonCount);
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];
	int vertexCounter = 0; //just a counter, keeps track of which vertex is current

	//how many polygons in mesh
	for (int i = 0; i < lPolygonCount; i++)
	{
		int lPolygonSize = pMesh->GetPolygonSize(i);
		m_mesh.setNrOfVerticesPerPolygon(lPolygonSize); // if triangulated = 3

		//where the i polygon start to read vertices from GetPolygonVertex
		int lStartIndex = pMesh->GetPolygonVertexIndex(i);

		//how many vertices in polygon, depends if triangulated
		for (int j = 0; j < lPolygonSize; j++)
		{
			//i = what polygon, j = vertice in polygon
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			m_mesh.AddIndexPoint(lControlPointIndex);

			//how many UV coordinates the polygon has, 1 right now
			for (int k = 0; k < pMesh->GetElementUVCount(); ++k)
			{
				int boi = pMesh->GetElementUVCount();
				FbxGeometryElementUV* lEUV = pMesh->GetElementUV(k);

				switch (lEUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint: //if its mapped by control point
					switch (lEUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display2DVector(header, lEUV->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = lEUV->GetIndexArray().GetAt(lControlPointIndex);
						Display2DVector(header, lEUV->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break;
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:	//or by vertex, used right now
				{
					int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
					m_mesh.AddUVIndex(lTextureUVIndex);

					switch (lEUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect: 
					{
						//actually does something, corrupted before, but is probably fine now.
						FbxVector2 textureUvCoordinates = lEUV->GetDirectArray().GetAt(lTextureUVIndex);
						m_mesh.AddUVCoordinate(textureUvCoordinates);
					}
					break;
					default:
						break;
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: //never used
				case FbxGeometryElement::eAllSame:	 //-||-
				case FbxGeometryElement::eNone:		 //-||-
					break;
				}
			}
			//How many normals per Polygon, 1 right now
			for (int k = 0; k < pMesh->GetElementNormalCount(); ++k)
			{
				FbxVector4 lNormalCoordinates;
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(k);

				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect: //currently used
						lNormalCoordinates = leNormal->GetDirectArray().GetAt(vertexCounter);
						m_mesh.AddNormalCoordinate(lNormalCoordinates);
						vertexCounter++;

						//Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						//int id = leNormal->GetIndexArray().GetAt(vertexId);
						//Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
		}
	}
}

void SaveHierarchy::SaveMeshName(FbxNode* pNode)
{
	const char* nodeName = pNode->GetName();	//The node we get right now is the pCube1 which is the name of the cube in the outliner
}

void SaveHierarchy::PrintChildName(FbxMesh* pMesh)
{
	printf("%s", pMesh->GetName());
}