#include "SaveHierarchy.h"

SaveHierarchy::SaveHierarchy()
{
	//m_mesh = MeshData2();
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
			m_SaveNode(lRootNode->GetChild(i));
		}
	}
}

void SaveHierarchy::m_SaveNode(FbxNode* pNode)
{
	FbxNodeAttribute::EType nodeType = pNode->GetNodeAttributeByIndex(0)->GetAttributeType();

	//Check all if they are static meshes first, put them in the file first.
	//Then hitboxes
	//then dynamic meshes
	//then material

	bool collisionBool = false;
	FbxProperty collision = pNode->FindProperty("Collision", true);
	if (collision.IsValid())
	{
		FbxBool collisionBoolFbx = collision.Get<bool>();
		collisionBool = collisionBoolFbx;
		m_staticMesh.setCollision(collisionBool);
	}
	else
	{
		printf("Collision Bool not found");
	}

	bool staticMeshBool = false;
	FbxProperty staticMesh = pNode->FindProperty("StaticMesh", true);
	if (staticMesh.IsValid())
	{
		FbxBool staticMeshBoolFbx = staticMesh.Get<bool>();
		staticMeshBool = staticMeshBoolFbx;
		m_staticMesh.setStaticMesh(staticMeshBool);
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
			//SaveBoundingBoxMesh
			m_SaveHitboxMesh(pNode);	//saves relevant into in m_mesh
			m_file.WriteBoundingBoxMesh(m_bBMesh);	//sends m_mesh to file writer for static mesh
			m_bBMesh.PrepareForNewMesh();
		}
		else
		{
			if (staticMeshBool)   //if its static
			{
				m_SaveStaticMesh(pNode);	//saves relevant into in m_mesh
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
		m_SaveNode(pNode->GetChild(j));	//Deeper in the tree
	}
}

void SaveHierarchy::m_SaveControlPoints(FbxMesh* pMesh)
{
	int lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	for (int i = 0; i < lControlPointsCount; i++)
	{
		m_staticMesh.AddControlPoint(lControlPoints[i]);
	}
	printf("\n\n");
}

void SaveHierarchy::m_SaveControlPointsIndex(FbxMesh* pMesh, int i, int j)
{
	int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
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
void SaveHierarchy::m_SaveStaticMesh(FbxNode* pNode) //trying to make SavePolygons into functions 
{
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	int lPolygonCount = lMesh->GetPolygonCount();
	int lVertexCounter = 0;
	int lPolygonSize = lMesh->GetPolygonSize(0); //checks first polygon, all should be 3

	m_staticMesh.setNrOfVerticesPerPolygon(lPolygonSize); //save in m_mesh
	m_staticMesh.setNrOfPolygons(lPolygonCount);	//save in m_mesh
	m_SaveMeshName(pNode);
	m_SaveControlPoints(lMesh);	//Save all controlpoints, to be used by index arr

	//Go through all polygons
	for (int i = 0; i < lPolygonCount; i++)
	{
		//Go through each vertice in the polygon
		for (int j = 0; j < lPolygonSize; j++)
		{
			//Save Control Point index
			m_SaveControlPointsIndex(lMesh, i, j);

			//how many UV coordinates the vertice has, 1 right now
			for (int k = 0; k < lMesh->GetElementUVCount(); ++k)
			{
				m_SaveUVCoordinatesAndIndex(lMesh, k, i, j, lVertexCounter);
			}

			//How many normals per vertice, 1 right now
			for (int k = 0; k < lMesh->GetElementNormalCount(); ++k)
			{
				m_SaveNormals(lMesh, k, lVertexCounter);
			}
			lVertexCounter++;
			m_staticMesh.increaseVertexCount();
		}
	}
}

void SaveHierarchy::m_SaveHitboxMesh(FbxNode* pNode)
{
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	int lPolygonCount = lMesh->GetPolygonCount();
	int lVertexCounter = 0;
	int lPolygonSize = lMesh->GetPolygonSize(0); //checks first polygon, all should be 3

	//SaveMeshName(pNode);
	m_staticMesh.setNrOfVerticesPerPolygon(lPolygonSize); //save in m_mesh
	m_staticMesh.setNrOfPolygons(lPolygonCount);	//save in m_mesh
	m_SaveControlPoints(lMesh);	//Save all controlpoints, to be used by index arr

	//Go through all polygons
	for (int i = 0; i < lPolygonCount; i++)
	{
		//Go through each vertice in the polygon
		for (int j = 0; j < lPolygonSize; j++)
		{
			//Save Control Point index
			m_SaveControlPointsIndex(lMesh, i, j);

			//how many UV coordinates the vertice has, 1 right now
			for (int k = 0; k < lMesh->GetElementUVCount(); ++k)
			{
				m_SaveUVCoordinatesAndIndex(lMesh, k, i, j, lVertexCounter);
			}

			//How many normals per vertice, 1 right now
			for (int k = 0; k < lMesh->GetElementNormalCount(); ++k)
			{
				m_SaveNormals(lMesh, k, lVertexCounter);
			}
			lVertexCounter++;
			m_staticMesh.increaseVertexCount();
		}
	}
}

void SaveHierarchy::m_SaveMeshName(FbxNode* pNode)
{
	const char* nodeName = pNode->GetName();	//outliner name
	printf("Name: %s\n", nodeName);

	int nameSize = 0;
	for (int i = 0; i < 100; i++)
	{
		if (nodeName[i] == '\0') //Chars write \0 after its done
			i = 100;
		else
			nameSize++;
	}
	m_staticMesh.setName(nodeName, nameSize);
}

void SaveHierarchy::m_PrintChildName(FbxMesh* pMesh)
{
	printf("%s", pMesh->GetName());
}