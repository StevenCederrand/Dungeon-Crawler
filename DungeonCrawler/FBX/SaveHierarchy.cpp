#include "SaveHierarchy.h"

//Material test stuff
#define MAT_HEADER_LENGTH 200


SaveHierarchy::SaveHierarchy()
{
	m_nrOfNodes = 0;
	m_nrOfStaticMesh = 0;
	m_nrOfBoundingBox = 0;
	m_nrOfMaterial = 0;
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
		m_file.WriteMainHeader(m_nrOfStaticMesh, m_nrOfBoundingBox, m_nrOfMaterial); //MainHeader
		//printf("FINAL Number of Nodes: %i\n", m_nrOfNodes);

		//Write Static Meshes
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			m_SaveStaticMeshNode(lRootNode->GetChild(i));
		}

		//Write Dynamic Meshes
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			m_SaveDynamicMeshNode(lRootNode->GetChild(i));
		}

		//Write Material
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			m_SaveMaterial(lRootNode->GetChild(i));
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
	{
		m_nrOfStaticMesh++;
		CheckUniqueMaterial(pNode);
	}
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

void SaveHierarchy::m_SaveDynamicMeshNode(FbxNode* pNode)
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
				/*
				m_SaveStaticMesh(pNode, collisionBool, staticMeshBool);	//saves relevant info in m_mesh
				m_file.WriteStaticMesh(m_staticMesh);	//sends m_mesh to file writer for static mesh
				m_staticMesh.PrepareForNewMesh();
				*/
			}
			else  //dynamic
			{
				m_SaveDynamicMesh(pNode, collisionBool, staticMeshBool);
				m_file.WriteDynamicMesh();
				//FIX THE FBX FILE TO HAVE COLLISION STATIC MESH TO NON
			}
		}
		break;
	}

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		printf("\n");
		m_SaveDynamicMeshNode(pNode->GetChild(j));	//Deeper in the tree
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

void SaveHierarchy::m_SaveMaterial(FbxNode*pNode)
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
				//write materialInfo
				FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
				SaveMaterial(lMesh);

				//do check if this material has already been written

				bool exists = false;

				m_materialIDSent.push_back(m_Material.materialID);
				if (m_materialIDSent.size() >= 2)
				{
					for (int j = 0; j < m_materialIDSent.size() - 1; j++ && exists == false)
					{
						if (m_materialIDSent[m_materialIDSent.size() - 1] == m_materialIDSent[j])
						{
							exists = true;
						}
					}
				}

				if (exists == false)
				{
					m_file.WriteMaterial(m_Material);
				}
				else if (exists == true)
				{
					m_materialIDSent.pop_back();
				}

			}
			else  //dynamic
			{

			}
		}
		break;
	}

	m_ResetMaterial();

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		printf("\n");
		m_SaveMaterial(pNode->GetChild(j));	//Deeper in the tree
	}
}

void SaveHierarchy::m_ResetMaterial()
{
	for (int i = 0; i < 100; i++)
	{
		m_Material.nameOfAlbedo[i] = ' ';
		m_Material.nameOfNormal[i] = ' ';
	}
	m_Material.materialID = 0;
	m_Material.nrOfTextures = 0;
	m_Material.whatShader = 0;
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

void SaveHierarchy::m_SaveDynamicMesh(FbxNode* pNode, bool collision, bool staticMesh)
{

}

void SaveHierarchy::m_SaveHitboxMesh(FbxNode* pNode, bool collision, bool staticMesh)
{
	m_bBMesh.setCollision(collision);
	m_bBMesh.setStaticMesh(staticMesh);

	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	int lPolygonCount = lMesh->GetPolygonCount();	//SHOULD BE 12 IF HITBOX
	int lVertexCounterHit = 0;
	int lPolygonSize = lMesh->GetPolygonSize(0); //checks first polygon, all should be 3

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






//MATERIAL TEST STUFF
void SaveHierarchy::CheckUniqueMaterial(FbxNode* pNode) //checks if the meshes material is unique
{
	FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();

	//check whether the material maps with only one mesh
	bool lIsAllSame = true;
	for (int i = 0; i < pMesh->GetElementMaterialCount(); i++)
	{

		FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(i);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			lIsAllSame = false;
			break;
		}
	}

	//For eAllSame mapping type, just out the material and texture mapping info once
	if (lIsAllSame)
	{
		if (pMesh->GetElementMaterialCount() == 1)
		{
			for (int i = 0; i < pMesh->GetElementMaterialCount(); i++)
			{
				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(i);
				if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
				{
					FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
					int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
					if (lMatId >= 0)
					{
						int nr = lMaterial->GetUniqueID(); //MAKES UNIQUE ID
						m_uniqueMaterial.push_back(nr);

						bool exists = false;
						for (int j = 0; j < m_uniqueMaterial.size()-1; j++ && exists == false)
						{
							if (m_uniqueMaterial[m_uniqueMaterial.size()-1] == m_uniqueMaterial[j])
							{
								exists = true;
							}
						}
						if (exists == false)
						{
							m_nrOfMaterial++;
						}
					}
				}
			}
		}
		else if (pMesh->GetElementMaterialCount() == 0)		//no material
		{
			m_staticMesh.setMaterialID(-1);
		}
	}
	printf("nrOFmaterial %i \n", m_nrOfMaterial);
}

void SaveHierarchy::SaveMaterial(FbxMesh* pMesh)
{
	SaveMaterialConnections(pMesh);
}

void SaveHierarchy::SaveMaterialConnections(FbxMesh* pMesh)
{
	int i, l, lPolygonCount = pMesh->GetPolygonCount();

	char header[MAT_HEADER_LENGTH];

	DisplayString("    Polygons Material Connections");

	//check whether the material maps with only one mesh
	bool lIsAllSame = true;
	for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
	{

		FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			lIsAllSame = false;
			break;
		}
	}

	//For eAllSame mapping type, just out the material and texture mapping info once
	if (lIsAllSame)
	{
		if (pMesh->GetElementMaterialCount() != 0)
		{
			for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
			{

				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
				if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
				{
					FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
					int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
					if (lMatId >= 0)
					{
						int nr = lMaterial->GetUniqueID(); //GETS UNIQUE ID
						m_staticMesh.setMaterialID(nr);
						m_Material.materialID = nr;

						DisplayInt("        All polygons share the same material in mesh ", l);
						SaveMaterialTextureConnections(lMaterial, lMatId, l);
					}
				}
			}
		}
		//no material
		else if (pMesh->GetElementMaterialCount() == 0)		//no material
		{
			m_staticMesh.setMaterialID(-1);
			m_Material.materialID = -1;
		}
	}

	//For eByPolygon mapping type, just out the material and texture mapping info once
	else
	{
		for (i = 0; i < lPolygonCount; i++)
		{
			DisplayInt("        Polygon ", i);

			for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
			{

				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
				FbxSurfaceMaterial* lMaterial = NULL;
				int lMatId = -1;
				lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
				lMatId = lMaterialElement->GetIndexArray().GetAt(i);

				if (lMatId >= 0)
				{
					SaveMaterialTextureConnections(lMaterial, lMatId, l);
				}
			}
		}
	}
}

void SaveHierarchy::SaveMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, int pMatId, int l)
{
	if (!pMaterial)
		return;

	FbxProperty lProperty;

	//Diffuse Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	SaveTextureNames(lProperty, 1);

	//Normal Map Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
	SaveTextureNames(lProperty, 2);

	m_Material.whatShader = 1;
	//material ready to be written to file
}

void SaveHierarchy::SaveTextureNames(FbxProperty &pProperty, int mapKind)
{
	int lLayeredTextureCount = pProperty.GetSrcObjectCount<FbxLayeredTexture>();
	if (lLayeredTextureCount > 0)
	{
		for (int j = 0; j < lLayeredTextureCount; ++j)
		{
			FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
			int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();

			for (int k = 0; k < lNbTextures; ++k)
			{

			}
		}
	}
	else
	{
		//no layered texture simply get on the property
		int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();

		if (lNbTextures > 0)
		{

			for (int j = 0; j < lNbTextures; ++j)
			{
				FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
				FbxFileTexture* lTextureFile = pProperty.GetSrcObject<FbxFileTexture>(j);
				if (lTexture)
				{

					//the path to the file, save the last part only?
					std::string nameOfTexture = lTextureFile->GetFileName();
					int fileStart = nameOfTexture.find_last_of("/");
					std::string nameOfTextureLastPart = nameOfTexture.substr(fileStart + 1);
					int nameOfTextureLastPartLength = nameOfTextureLastPart.length();

					//1 = albedo 2 = normal
					if (mapKind == 1)
					{
						for (int k = 0; k < nameOfTextureLastPartLength; k++) //100 spaces in the variable
						{
							m_Material.nameOfAlbedo[k] = nameOfTextureLastPart[k];
						}
						for (int k = nameOfTextureLastPartLength; k < 100; k++)
						{
							m_Material.nameOfAlbedo[k] = ' ';
						}
						m_Material.nrOfTextures++;
					}
					else if (mapKind == 2)
					{
						for (int k = 0; k < nameOfTextureLastPartLength; k++) //100 spaces in the variable
						{
							m_Material.nameOfNormal[k] = nameOfTextureLastPart[k];
						}
						for (int k = nameOfTextureLastPartLength; k < 100; k++)
						{
							m_Material.nameOfNormal[k] = ' ';
						}
						m_Material.nrOfTextures++;
					}
				}
			}
		}
	}
}