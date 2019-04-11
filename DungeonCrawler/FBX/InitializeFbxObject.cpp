#include <fbxsdk.h>
#include "InitializeFbxObject.h"


InitializeFbxObject::InitializeFbxObject()
{
}

FbxManager *& InitializeFbxObject::CreateFbxManager()
{
	// Initialize the SDK manager. This object handles all our memory management.
	FbxManager* lSdkManager = FbxManager::Create();
	return lSdkManager;
}

FbxIOSettings *& InitializeFbxObject::CreateIOSettingsObject(FbxManager *& lSdkManager)
{
	// Create the IO settings object. This is mostly used when importing and exporting files.
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	return ios;
}

FbxScene *& InitializeFbxObject::CreateFbxScene(FbxManager * lSdkManager)
{
	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	return lScene;
}

FbxImporter *& InitializeFbxObject::CreateFbxImporter(FbxManager * lSdkManager)
{
	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	return lImporter;
}

const char * InitializeFbxObject::LoadFbxFile()
{
	//Loading in my file
	const char* lFilename = "\\Assets\\FBX\\severalObjects.fbx";
	return lFilename;
}

void InitializeFbxObject::InitializeFbxImporter(FbxImporter *& lImporter, const char * lFilename, FbxManager * lSdkManager)
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

void InitializeFbxObject::UseFbxImporter(FbxImporter *& lImporter, FbxScene *& lScene)
{
	// Import the contents of our fbx file into the scene. 
	// Our fbx is stored in the importer from the initialization of the importer.
	lImporter->Import(lScene);
}

void InitializeFbxObject::DestroyFbxImporter(FbxImporter *& lImporter)
{
	// The file is loaded into the scene and the importer is no longer needed, Destroy.
	lImporter->Destroy();
}
