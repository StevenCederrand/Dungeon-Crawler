#include <fbxsdk.h>

#include <string>
#include <fstream>
#include <iostream>

#define DELTA 0.0001
#define EQUAL(A,B) (abs((A)-(B)) < DELTA) ? true:false

/*
//Header Structs
struct MainHeader
{
	unsigned int meshCount;
	unsigned int materialCount;
	unsigned int characterCount;
};

struct MeshHeader 
{
	unsigned int vertexCount;
	unsigned int materialID; //our different morph bois will use the same material
	unsigned int UV[2]; //should this be here? int?
	char nameOfCustomAttribute[100]; //might not be needed
	bool collision; //custom maya attribute
};

struct BoundingBoxHeader
{
	unsigned int vertexCount;
};

//Small info structs
struct Vertex 
{
	float position[3];
	float normal[3];
	float tangent[3];
	float biTangent[3];
};

struct BoundingBoxVertex
{
	float position[3];
	//indices? faces?
};

class Material
{
	private:
		//texture buffer pointer
		//shader int pointer
};

class Texture
{
	private:
		//texture buffer
};

class ShaderOption
{
	private:
		//what shader it should use
};
*/
void createCustomFile()
{
	/*
	//IGNORE FOR NOW, START WITH FBX FILE

	//HeadersAndStuff
	MeshHeader h { 100 };
	Vertex *vArray = new Vertex[h.vertexCount];
	//Write to file
	std::ofstream outfile("testBin.bin", std::ofstream::binary); //make a new file, make sure to write binary
	outfile.write((const char*)&h, sizeof(MeshHeader));
	//add info to header
	outfile.write((const char*)vArray, sizeof(Vertex)*h.vertexCount);
	outfile.close();

	//Read from file
	std::ifstream infile("testBin.bin", std::ifstream::binary); //make sure to read binary

	//read the first 4 bytes and put into h2
	MeshHeader h2;
	infile.read((char*)&h2, sizeof(MeshHeader));


	h2.vertexCount; //- a lot
	//Default construction of Vertex struct
	Vertex *vertices = new Vertex[h2.vertexCount];
	//Read all the vertices on the file
	infile.read((char*)vertices, h2.vertexCount * sizeof(Vertex));
	infile.close();

	//comparison:
	bool equal = true;
	for (int i = 0; i < h.vertexCount; i++)
	{
		if (!EQUAL(vArray[i].position[0], vertices[i].position[0]) ||
			!EQUAL(vArray[i].position[1], vertices[i].position[1]) ||
			!EQUAL(vArray[i].position[2], vertices[i].position[2]))
		{
			equal = false;
			break;
		}
	}
	std::cout << "Streams are equal, method 2: " << equal << std::endl;
	*/
}
FbxManager* createFbxManager()
{
	// Initialize the SDK manager. This object handles all our memory management.
	FbxManager* lSdkManager = FbxManager::Create();
	return lSdkManager;
}

FbxIOSettings* createIOSettingsObject(FbxManager* lSdkManager)
{
	// Create the IO settings object. This is mostly used when importing and exporting files.
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	return ios;
}

FbxScene* createFbxScene(FbxManager* lSdkManager)
{
	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	return lScene;
}

FbxImporter* createFbxImporter(FbxManager* lSdkManager)
{
	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	return lImporter;
}

const char* loadFbxFile()
{
	//Loading in my file
	const char* lFilename = "\\Assets\\FBX\\box.fbx";
	return lFilename;
}

void initializeFbxImporter(FbxImporter* lImporter, const char* lFilename, FbxManager* lSdkManager)
{
	// Use the first argument as the name for our FBX file. Secon d is fileFormat, leave at -1. Last is what IO Settings to use.
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) //If initializing failes, go into if statement
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
}

void useFbxImporter(FbxImporter* lImporter, FbxScene* lScene)
{
	// Import the contents of the file into the scene.
	lImporter->Import(lScene);
}

//Tab character ("\t") counter
int m_numTabs = 0;

//prints tabs dependent on variable
void PrintTabs() 
{
	for (int i = 0; i < m_numTabs; i++)
		printf("\t");	//\t = tab, like several spaces
}


// Return a string-based representation based on the attribute type.
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) 
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
void PrintAttribute(FbxNodeAttribute* pAttribute) 
{
	if (!pAttribute) return;

	//Fbx Strings, to retrieve the character array of a FbxString, use its Buffer() method.
	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType()); //uses the big list
	FbxString attrName = pAttribute->GetName();

	//std Strings
	std::string typeNameString = typeName.Buffer();
	std::string attrNameString = attrName.Buffer();

	//prints tabs, unsure why
	PrintTabs();

	//Check if the strings exist
	if (typeNameString != "")	//this needs to use the std string
		printf("\nType Name: %s", typeName.Buffer());	//this needs to use the fbx string
	else
		printf("\nNo Type Name.");
	
	if (attrNameString != "")
		printf("\nAttribute Name: %s", attrName.Buffer());
	else
		printf("\nNo Attribute Name.");
}

//print node, its attributes and its children, recursively.
void PrintNode(FbxNode* pNode) 
{
	PrintTabs();
	const char* nodeName = pNode->GetName();
	
	FbxDouble3 translation = pNode->LclTranslation.Get();
	FbxDouble3 rotation = pNode->LclRotation.Get();
	FbxDouble3 scaling = pNode->LclScaling.Get();

	// Print the contents of the node. Need to declare spaces for variables first!
	printf("Name: %s\nTranslation: %f %f %f\nRotation: %f %f %f\nScaling: %f %f %f\n", nodeName,
		translation[0], translation[1], translation[2],
		rotation[0], rotation[1], rotation[2],
		scaling[0], scaling[1], scaling[2]);
	m_numTabs++;

	// Print the node's attributes.
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
		PrintNode(pNode->GetChild(j));

	m_numTabs--;
	PrintTabs();
	printf("\n");
}


int main(int argc, char** argv) 
{
	//createCustomFile();

	// 0: Start with this, FBX SDK

	//Create usefull Objects
	FbxManager* lSdkManager = createFbxManager();
	FbxIOSettings* ios = createIOSettingsObject(lSdkManager);
	FbxScene* lScene = createFbxScene(lSdkManager);
	FbxImporter* lImporter = createFbxImporter(lSdkManager);

	const char* lFilename = loadFbxFile();

	initializeFbxImporter(lImporter, lFilename, lSdkManager); //Importer Lifecycle: Create, Initialize, Use, Destroy.

	// Import the contents of the file into the scene.
	useFbxImporter(lImporter, lScene);

	// The file is imported; so get rid of the importer.
	lImporter->Destroy();

	// Print the nodes of the scene and their attributes recursively.
	// Note that we are not printing the root node because it should not contain any attributes.
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) 
	{
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			PrintNode(lRootNode->GetChild(i));
	}

	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();
	return 0;
}