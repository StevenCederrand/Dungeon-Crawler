#pragma once

#include <fbxsdk.h>
#include <vector>
#include "FileExplorer.h"

class fbxObjectManager
{
public:
	fbxObjectManager();
	~fbxObjectManager();
	void initializer();
	FbxManager*& CreateFbxManager();
	FbxIOSettings*& CreateIOSettingsObject(FbxManager*& lSdkManager);
	FbxScene*& CreateFbxScene(FbxManager* lSdkManager);
	FbxImporter*& CreateFbxImporter(FbxManager* lSdkManager);
	std::string LoadFbxFile();
	void InitializeFbxImporter(FbxImporter*& lImporter, std::string lnameOfFile, FbxManager* lSdkManager);
	void UseFbxImporter(FbxImporter*& lImporter, FbxScene*& lScene);
	void DestroyFbxImporter(FbxImporter*& lImporter);
	FbxScene* GetScene()const;

private:
	FbxManager* m_lsdkManager;
	FbxScene* m_lsdkScene;
	FbxImporter* m_lsdkImporter;
	std::string m_lFilename;

};