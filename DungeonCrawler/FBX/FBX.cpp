#include <fbxsdk.h>

#include <string>
#include <fstream>
#include <iostream>

#define DELTA 0.0001
#define EQUAL(A,B) (abs((A)-(B)) < DELTA) ? true:false

//Declerations of all functions
void createCustomFile();
FbxManager* createFbxManager();
FbxIOSettings* createIOSettingsObject(FbxManager* lSdkManager);
FbxScene* createFbxScene(FbxManager* lSdkManager);
FbxImporter* createFbxImporter(FbxManager* lSdkManager);
const char* loadFbxFile();
void initializeFbxImporter(FbxImporter* lImporter, const char* lFilename, FbxManager* lSdkManager);
void useFbxImporter(FbxImporter* lImporter, FbxScene* lScene);
void destroyFbxImporter(FbxImporter* lImporter);
void PrintNode(FbxNode* pNode);
void printAllNodes(FbxScene* lScene);
void PrintTabs();
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
void PrintAttribute(FbxNodeAttribute* pAttribute);

int m_numTabs = 0; //Tab character ("\t") counter

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
	const char* lFilename = "\\Assets\\FBX\\severalObjects.fbx";
	return lFilename;
}

void initializeFbxImporter(FbxImporter* lImporter, const char* lFilename, FbxManager* lSdkManager)
{
	// Use the first argument as the name for our FBX file.
	// Second is fileFormat, leave at -1.
	// Last is what IO Settings to use, we get it from out manager which in turn gets it from our IOSettingsObject. Default settings are used.
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) //If initializing failes, go into if statement
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
}

void useFbxImporter(FbxImporter* lImporter, FbxScene* lScene)
{
	// Import the contents of our fbx file into the scene. 
	// Our fbx is stored in the importer from the initialization of the importer.
	lImporter->Import(lScene);
}

void destroyFbxImporter(FbxImporter* lImporter)
{
	// The file is loaded into the scene and the importer is no longer needed, Destroy.
	lImporter->Destroy();
}

void printAllNodes(FbxScene* lScene)
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
void PrintNode(FbxNode* pNode)
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

	// Print the node's attributes.
	for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		PrintAttribute(pNode->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		printf("\n");
		m_numTabs++; //Goes deeper into the nodetree
		PrintNode(pNode->GetChild(j));	//Deeper in the tree
	}

	m_numTabs--; //Goes out of the nodetree
}

//prints tabs dependent on variable, just for better reading of info in consol
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
	useFbxImporter(lImporter, lScene);
	destroyFbxImporter(lImporter);

	printAllNodes(lScene);
	
	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();
	return 0;
}