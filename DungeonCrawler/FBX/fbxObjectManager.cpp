#include "fbxObjectManager.h"

fbxObjectManager::fbxObjectManager()
{
	initializer();
}

fbxObjectManager::~fbxObjectManager()
{
	m_lsdkScene->Destroy();
	m_lsdkManager->Destroy();
}

void fbxObjectManager::initializer()
{
	this->m_lsdkManager = CreateFbxManager();
	CreateIOSettingsObject(m_lsdkManager);
	this->m_lsdkScene = CreateFbxScene(m_lsdkManager);
	this->m_lsdkImporter = CreateFbxImporter(m_lsdkManager);
	m_lFilename = LoadFbxFile();
	InitializeFbxImporter(m_lsdkImporter, m_lFilename, m_lsdkManager);
	UseFbxImporter(m_lsdkImporter, m_lsdkScene);
	DestroyFbxImporter(m_lsdkImporter);
}

FbxManager *& fbxObjectManager::CreateFbxManager()
{
	// Initialize the SDK manager. This object handles all our memory management.
	FbxManager* lSdkManager = FbxManager::Create();
	return lSdkManager;
}

FbxIOSettings *& fbxObjectManager::CreateIOSettingsObject(FbxManager *& lSdkManager)
{
	// Create the IO settings object. This is mostly used when importing and exporting files.
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	return ios;
}

FbxScene *& fbxObjectManager::CreateFbxScene(FbxManager * lSdkManager)
{
	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	return lScene;
}

FbxImporter *& fbxObjectManager::CreateFbxImporter(FbxManager * lSdkManager)
{
	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	return lImporter;
}

std::string fbxObjectManager::LoadFbxFile()
{
	FileExplorer fileExplorer;
	std::string nameOfFile = fileExplorer.nameOfFileToOpen();
	
	return nameOfFile;
}

void fbxObjectManager::InitializeFbxImporter(FbxImporter *& lImporter, std::string lnameOfFile, FbxManager * lSdkManager)
{
	// Use the first argument as the name for our FBX file.
	// Second is fileFormat, leave at -1.
	// Last is what IO Settings to use, we get it from out manager which in turn gets it from our IOSettingsObject. Default settings are used.
	if (!lImporter->Initialize(lnameOfFile.c_str(), -1, lSdkManager->GetIOSettings())) //If initializing failes, go into if statement
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
}

void fbxObjectManager::UseFbxImporter(FbxImporter *& lImporter, FbxScene *& lScene)
{
	// Import the contents of our fbx file into the scene. 
	// Our fbx is stored in the importer from the initialization of the importer.
	lImporter->Import(lScene);
}

void fbxObjectManager::DestroyFbxImporter(FbxImporter *& lImporter)
{
	// The file is loaded into the scene and the importer is no longer needed, Destroy.
	lImporter->Destroy();
}

FbxScene* fbxObjectManager::GetScene() const
{
	return m_lsdkScene;
}