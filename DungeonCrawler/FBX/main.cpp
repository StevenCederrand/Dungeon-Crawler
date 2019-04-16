#include <fbxsdk.h>

//Some of DisplayCommon functions have memory leaks, be wary of activating them!

#include "fbxObjectManager.h"
#include "DisplayHierarchy.h"
#include "WriteCustomFile.h"
#include "SaveHierarchy.h"

//#include "MeshData2.h" //WARNING

int main(int argc, char** argv) 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	
	//Make objects like SDK manager, scene, importer
	//Imports FBX file and cleans importer
	fbxObjectManager lsdkObject = fbxObjectManager();

	//Display our FBX file hierarchy
	DisplayHierarchy displayHierarchyObject;
	//displayHierarchyObject.DisplayEntireHierarchy(lsdkObject.GetScene());

	//Create our custom file
	WriteCustomFile writeCustomFileObject;
	//writeCustomFileObject.CreateCustomFile();
	
	SaveHierarchy saveHierarchy;
	//saveHierarchy.SaveEntireHierarchy(lsdkObject.GetScene());




	int stop = 1;

	//MESHDATA2 IS CORRUPTED

	return 0;
}