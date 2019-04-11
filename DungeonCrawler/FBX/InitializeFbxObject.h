#pragma once
#ifndef _INITIALIZEFBXOBJECT_H
#define _INITIALIZEFBXOBJECT_H

class InitializeFbxObject
{
public:
	InitializeFbxObject();
	void initializer();
	FbxManager*& CreateFbxManager();
	FbxIOSettings*& CreateIOSettingsObject(FbxManager*& lSdkManager);
	FbxScene*& CreateFbxScene(FbxManager* lSdkManager);
	FbxImporter*& CreateFbxImporter(FbxManager* lSdkManager);
	const char* LoadFbxFile();
	void InitializeFbxImporter(FbxImporter*& lImporter, const char* lFilename, FbxManager* lSdkManager);
	void UseFbxImporter(FbxImporter*& lImporter, FbxScene*& lScene);
	void DestroyFbxImporter(FbxImporter*& lImporter);
	FbxScene* GetScene()const;

private:
	FbxManager* m_lsdkManager;
	FbxScene* m_lsdkScene;
	FbxImporter* m_lsdkImporter;
	const char* m_lFilename;

};
#endif _INITIALIZEFBXOBJECT_H