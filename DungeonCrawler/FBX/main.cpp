#include <fbxsdk.h>

#include "DisplayCommon.h"
#include "InitializeFbxObject.h"
#include "DisplayHierarchy.h"

#define DELTA 0.0001
#define EQUAL(A,B) (abs((A)-(B)) < DELTA) ? true:false

//Declerations of all functions
void CreateCustomFile();
FbxManager*& CreateFbxManager();
FbxIOSettings*& CreateIOSettingsObject(FbxManager*& lSdkManager);
FbxScene*& CreateFbxScene(FbxManager* lSdkManager);
FbxImporter*& CreateFbxImporter(FbxManager* lSdkManager);
const char* LoadFbxFile();
void InitializeFbxImporter(FbxImporter*& lImporter, const char* lFilename, FbxManager* lSdkManager);
void UseFbxImporter(FbxImporter*& lImporter, FbxScene*& lScene);
void DestroyFbxImporter(FbxImporter*& lImporter);

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
void CreateCustomFile()
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
FbxManager*& CreateFbxManager()
{
	// Initialize the SDK manager. This object handles all our memory management.
	FbxManager* lSdkManager = FbxManager::Create();
	return lSdkManager;
}

FbxIOSettings*& CreateIOSettingsObject(FbxManager*& lSdkManager)
{
	// Create the IO settings object. This is mostly used when importing and exporting files.
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	return ios;
}

FbxScene*& CreateFbxScene(FbxManager* lSdkManager)
{
	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	return lScene;
}

FbxImporter*& CreateFbxImporter(FbxManager* lSdkManager)
{
	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	return lImporter;
}

const char* LoadFbxFile()
{
	//Loading in my file
	const char* lFilename = "\\Assets\\FBX\\doubleBox.fbx";
	return lFilename;
}

void InitializeFbxImporter(FbxImporter*& lImporter, const char* lFilename, FbxManager* lSdkManager)
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

void UseFbxImporter(FbxImporter*& lImporter, FbxScene*& lScene)
{
	// Import the contents of our fbx file into the scene. 
	// Our fbx is stored in the importer from the initialization of the importer.
	lImporter->Import(lScene);
}

void DestroyFbxImporter(FbxImporter*& lImporter)
{
	// The file is loaded into the scene and the importer is no longer needed, Destroy.
	lImporter->Destroy();
}


int main(int argc, char** argv) 
{
	//createCustomFile();

	// 0: Start with this, FBX SDK
	//Create usefull Objects
	//FbxManager* lSdkManager = CreateFbxManager();
	//FbxIOSettings* ios = CreateIOSettingsObject(lSdkManager);
	//FbxScene* lScene = CreateFbxScene(lSdkManager);
	//FbxImporter* lImporter = CreateFbxImporter(lSdkManager);


	//const char* lFilename = LoadFbxFile();

	//InitializeFbxImporter(lImporter, lFilename, lSdkManager); //Importer Lifecycle: Create, Initialize, Use, Destroy.
	//UseFbxImporter(lImporter, lScene);
	//DestroyFbxImporter(lImporter);
	
	InitializeFbxObject lsdkObject = InitializeFbxObject();

	DisplayHierarchy displayHierarchyObject;
	displayHierarchyObject.DisplayEntireHierarchy(lsdkObject.GetScene());
	
	// Destroy the SDK manager and all the other objects it was handling.
	//lSdkManager->Destroy();
	return 0;
}