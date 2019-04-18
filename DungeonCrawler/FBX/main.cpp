#include <fbxsdk.h>

//Some of DisplayCommon functions have memory leaks, be wary of activating them!

#include "fbxObjectManager.h"
#include "DisplayHierarchy.h"
#include "SaveHierarchy.h"

int main(int argc, char** argv) 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	
	//Make objects like SDK manager, scene, importer
	//Imports FBX file and cleans importer
	fbxObjectManager lsdkObject = fbxObjectManager();

	//Display our FBX file hierarchy
	DisplayHierarchy displayHierarchyObject;
	//displayHierarchyObject.DisplayEntireHierarchy(lsdkObject.GetScene());
	
	SaveHierarchy saveHierarchy;
	saveHierarchy.SaveEntireHierarchy(lsdkObject.GetScene());

	int stop = 1;

	return 0;
}