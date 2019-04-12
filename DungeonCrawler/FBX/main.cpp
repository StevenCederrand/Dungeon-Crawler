#include <fbxsdk.h>

#include "fbxObjectManager.h"
#include "DisplayHierarchy.h"
#include "WriteCustomFile.h"
#include "SaveMeshInfo.h"

int main(int argc, char** argv) 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Make objects like SDK manager, scene, importer
	//Imports FBX file and cleans importer
	fbxObjectManager lsdkObject = fbxObjectManager();

	//Display our FBX file hierarchy
	DisplayHierarchy displayHierarchyObject;
	displayHierarchyObject.DisplayEntireHierarchy(lsdkObject.GetScene());

	//Create our custom file
	WriteCustomFile writeCustomFileObject;
	writeCustomFileObject.CreateCustomFile();
	
	return 0;
}