#include "SaveHierarchy.h"

SaveHierarchy::SaveHierarchy()
{
	m_nrOfNodes = 0;
	m_nrOfStaticMesh = 0;
	m_nrOfBoundingBox = 0;
}

SaveHierarchy::~SaveHierarchy()
{

}

void SaveHierarchy::SaveEntireHierarchy(FbxScene* lScene)
{
	// Note that we are not printing the root node because it should not contain any attributes.
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode)
	{
		//Count how many number of nodes we have to work with
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			m_calculateNrOfNodes(lRootNode->GetChild(i));
		}
		m_file.WriteMainHeader(m_nrOfStaticMesh, m_nrOfBoundingBox); //MainHeader
		//printf("FINAL Number of Nodes: %i\n", m_nrOfNodes);

		//Write Meshes
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			m_SaveStaticMeshNode(lRootNode->GetChild(i));
		}

		//Write Hitboxes
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			m_SaveStaticHitboxNode(lRootNode->GetChild(i));
		}
	}
}

void SaveHierarchy::m_calculateNrOfNodes(FbxNode* pNode)
{
	m_nrOfNodes++;

	//check which nodes are staticMeshes and which are bounding boxes
	bool collisionBool = false;
	bool staticMeshBool = false;

	FbxProperty collision = pNode->FindProperty("Collision", true);
	if (collision.IsValid())
	{
		FbxBool collisionBoolFbx = collision.Get<bool>();
		collisionBool = collisionBoolFbx;
	}
	else
	{
		printf("Collision Bool not found");
	}

	FbxProperty staticMesh = pNode->FindProperty("StaticMesh", true);
	if (staticMesh.IsValid())
	{
		FbxBool staticMeshBoolFbx = staticMesh.Get<bool>();
		staticMeshBool = staticMeshBoolFbx;
	}
	else
	{
		printf("Static Mesh Bool not found");
	}

	if (collisionBool)
		m_nrOfBoundingBox++;
	else
		m_nrOfStaticMesh++;

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		m_calculateNrOfNodes(pNode->GetChild(j));	//Deeper in the tree
	}
}

void SaveHierarchy::m_SaveStaticMeshNode(FbxNode* pNode)
{
	FbxNodeAttribute::EType nodeType = pNode->GetNodeAttributeByIndex(0)->GetAttributeType();
	bool collisionBool = false;
	bool staticMeshBool = false;

	FbxProperty collision = pNode->FindProperty("Collision", true);
	if (collision.IsValid())
	{
		FbxBool collisionBoolFbx = collision.Get<bool>();
		collisionBool = collisionBoolFbx;
	}
	else
	{
		printf("Collision Bool not found");
	}

	FbxProperty staticMesh = pNode->FindProperty("StaticMesh", true);
	if (staticMesh.IsValid())
	{
		FbxBool staticMeshBoolFbx = staticMesh.Get<bool>();
		staticMeshBool = staticMeshBoolFbx;
	}
	else
	{
		printf("Static Mesh Bool not found");
	}

	switch (nodeType)
	{
	default:
		break;
	case FbxNodeAttribute::eMesh:	//if its a mesh
		if (collisionBool)
		{

		}
		else
		{
			if (staticMeshBool)   //if its static
			{
				m_SaveStaticMesh(pNode, collisionBool, staticMeshBool);	//saves relevant info in m_mesh
				m_file.WriteStaticMesh(m_staticMesh);	//sends m_mesh to file writer for static mesh
				m_staticMesh.PrepareForNewMesh();
			}
			else  //dynamic
			{

			}
		}
		break;
	}

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		printf("\n");
		m_SaveStaticMeshNode(pNode->GetChild(j));	//Deeper in the tree
	}
}

void SaveHierarchy::m_SaveStaticHitboxNode(FbxNode* pNode)
{
	FbxNodeAttribute::EType nodeType = pNode->GetNodeAttributeByIndex(0)->GetAttributeType();
	bool collisionBool = false;
	bool staticMeshBool = false;

	FbxProperty collision = pNode->FindProperty("Collision", true);
	if (collision.IsValid())
	{
		FbxBool collisionBoolFbx = collision.Get<bool>();
		collisionBool = collisionBoolFbx;
	}
	else
	{
		printf("Collision Bool not found");
	}

	FbxProperty staticMesh = pNode->FindProperty("StaticMesh", true);
	if (staticMesh.IsValid())
	{
		FbxBool staticMeshBoolFbx = staticMesh.Get<bool>();
		staticMeshBool = staticMeshBoolFbx;
	}
	else
	{
		printf("Static Mesh Bool not found");
	}

	switch (nodeType)
	{
	default:
		break;
	case FbxNodeAttribute::eMesh:	//if its a mesh
		if (collisionBool)
		{
			m_SaveHitboxMesh(pNode, collisionBool, staticMeshBool);	//saves relevant into in m_mesh
			m_file.WriteBoundingBoxMesh(m_bBMesh);	//sends m_mesh to file writer for static mesh
			m_bBMesh.PrepareForNewMesh();
		}
		else
		{
			if (staticMeshBool)   //if its static
			{

			}
			else  //dynamic
			{

			}
		}
		break;
	}

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		printf("\n");
		m_SaveStaticHitboxNode(pNode->GetChild(j));	//Deeper in the tree
	}
}

void SaveHierarchy::m_SaveControlPoints(FbxMesh* pMesh, bool collision)
{
	int lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	if (collision)
	{
		for (int i = 0; i < lControlPointsCount; i++)
		{
			m_bBMesh.AddControlPoint(lControlPoints[i]);
		}
	}
	else
	{
		for (int i = 0; i < lControlPointsCount; i++)
		{
			m_staticMesh.AddControlPoint(lControlPoints[i]);
		}
	}
	printf("\n\n");
}

void SaveHierarchy::m_SaveControlPointsIndex(FbxMesh* pMesh, int i, int j, bool collision)
{
	int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
	if (collision)
		m_bBMesh.AddIndexPoint(lControlPointIndex);
	else
		m_staticMesh.AddIndexPoint(lControlPointIndex);

}

void SaveHierarchy::m_SaveUVCoordinatesAndIndex(FbxMesh* pMesh, int k, int i, int j, int vertexCounter)
{
	FbxGeometryElementUV* lEUV = pMesh->GetElementUV(k);

	switch (lEUV->GetMappingMode())
	{
	default:
		break;
	case FbxGeometryElement::eByControlPoint: //if its mapped by control point
		switch (lEUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			//Display2DVector(header, lEUV->GetDirectArray().GetAt(lControlPointIndex));
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			//int id = lEUV->GetIndexArray().GetAt(lControlPointIndex);
			//Display2DVector(header, lEUV->GetDirectArray().GetAt(id));
		}
		break;
		default:
			break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:	//or by vertex, used right now
	{
		int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j); //CORRECT

		switch (lEUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			//actually does something, corrupted before, but is probably fine now.
			FbxVector2 textureUvCoordinates = lEUV->GetDirectArray().GetAt(vertexCounter);
			m_staticMesh.AddUVCoordinate(textureUvCoordinates);
			m_staticMesh.AddUVIndex(lTextureUVIndex);
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

void SaveHierarchy::m_SaveNormals(FbxMesh* pMesh, int k, int vertexCounter)
{
	FbxVector4 lNormalCoordinates;
	FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(k);

	if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (leNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect: //currently used
			lNormalCoordinates = leNormal->GetDirectArray().GetAt(vertexCounter);
			m_staticMesh.AddNormalCoordinate(lNormalCoordinates); //save normals

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

//saves UV, UVIndex, normal, controlpointIndex
void SaveHierarchy::m_SaveStaticMesh(FbxNode* pNode, bool collision, bool staticMesh) //trying to make SavePolygons into functions 
{
	m_staticMesh.setCollision(collision);
	m_staticMesh.setStaticMesh(staticMesh);

	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	int lPolygonCount = lMesh->GetPolygonCount();
	int lVertexCounterStatic = 0;
	int lPolygonSize = lMesh->GetPolygonSize(0); //checks first polygon, all should be 3

	m_staticMesh.setNrOfVerticesPerPolygon(lPolygonSize); //save in m_mesh
	m_staticMesh.setNrOfPolygons(lPolygonCount);	//save in m_mesh
	m_SaveMeshName(pNode, collision);
	m_SaveControlPoints(lMesh, collision);	//Save all controlpoints, to be used by index arr

	int lNrOfVertices = 0;

	//Go through all polygons
	if (lPolygonSize == 3)
	{
		for (int i = 0; i < lPolygonCount; i++)
		{
			//Go through each vertice in the polygon
			for (int j = 0; j < lPolygonSize; j++)
			{
				//Save Control Point index
				m_SaveControlPointsIndex(lMesh, i, j, collision);

				//how many UV coordinates the vertice has, 1 right now
				for (int k = 0; k < lMesh->GetElementUVCount(); ++k)
				{
					m_SaveUVCoordinatesAndIndex(lMesh, k, i, j, lVertexCounterStatic);
				}

				//How many normals per vertice, 1 right now
				for (int k = 0; k < lMesh->GetElementNormalCount(); ++k)
				{
					m_SaveNormals(lMesh, k, lVertexCounterStatic);
				}
				lVertexCounterStatic++;

				lNrOfVertices++;
			}
		}
	}
	else
	{
		printf("WARNING, MESH NOT TRIANGULATED\n");
	}
	m_staticMesh.MakeAllTheVertices(lNrOfVertices); //ASSEMPLES ALL THE VERTICES
}

void SaveHierarchy::m_SaveHitboxMesh(FbxNode* pNode, bool collision, bool staticMesh)
{
	m_bBMesh.setCollision(collision);
	m_bBMesh.setStaticMesh(staticMesh);

	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	int lPolygonCount = lMesh->GetPolygonCount();	//SHOULD BE 12 IF HITBOX
	int lVertexCounterHit = 0;
	int lPolygonSize = lMesh->GetPolygonSize(0); //checks first polygon, all should be 3

	m_bBMesh.setNrOfVerticesPerPolygon(lPolygonSize); //save in m_mesh
	m_bBMesh.setNrOfPolygons(lPolygonCount);	//save in m_mesh
	m_SaveMeshName(pNode, collision);
	m_SaveControlPoints(lMesh, collision);	//Save all controlpoints, to be used by index arr

	int lNrOfVertices = 0;


	if (lPolygonSize == 3)
	{
		if (lPolygonCount == 12)
		{
			//Go through all polygons
			for (int i = 0; i < lPolygonCount; i++)
			{
				//Go through each vertice in the polygon
				for (int j = 0; j < lPolygonSize; j++)
				{
					//Save Control Point index
					m_SaveControlPointsIndex(lMesh, i, j, collision);

					lVertexCounterHit++;

					lNrOfVertices++;
				}
			}
		}
		else
		{
			printf("WARNING, HITBOX SHOULD HAVE EXACTLY 12 POLYGONS\n");
		}
	}
	else
	{
		printf("WARNING, MESH NOT TRIANGULATED\n");
	}
	m_bBMesh.MakeAllTheVertices(lNrOfVertices); //ASSEMPLES ALL THE VERTICES
}

void SaveHierarchy::m_SaveMeshName(FbxNode* pNode, bool collision)
{
	const char* nodeName = pNode->GetName();	//outliner name
	int nameSize = 0;

	for (int i = 0; i < 100; i++)
	{
		if (nodeName[i] == '\0') //Chars write \0 after its done
			i = 100;
		else
			nameSize++;
	}

	if (collision)
		m_bBMesh.setName(nodeName, nameSize);
	else
		m_staticMesh.setName(nodeName, nameSize);
}

void SaveHierarchy::m_PrintChildName(FbxMesh* pMesh)
{
	printf("%s", pMesh->GetName());
}