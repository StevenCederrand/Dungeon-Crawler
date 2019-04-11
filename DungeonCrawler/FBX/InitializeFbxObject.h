#pragma once

#ifndef _INITIALIZEFBXOBJECT_H
#define _INITIALIZEFBXOBJECT_H
class InitializeFbxObject
{
public:
	InitializeFbxObject();
	FbxManager*& CreateFbxManager();
	FbxIOSettings*& CreateIOSettingsObject(FbxManager*& lSdkManager);
	FbxScene*& CreateFbxScene(FbxManager* lSdkManager);
	FbxImporter*& CreateFbxImporter(FbxManager* lSdkManager);
	const char* LoadFbxFile();
	void InitializeFbxImporter(FbxImporter*& lImporter, const char* lFilename, FbxManager* lSdkManager);
	void UseFbxImporter(FbxImporter*& lImporter, FbxScene*& lScene);
	void DestroyFbxImporter(FbxImporter*& lImporter);

private:


};
#endif _INITIALIZEFBXOBJECT_H