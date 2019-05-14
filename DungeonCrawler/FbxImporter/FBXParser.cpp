#include "FBXParser.h"

//NOT USED ATM

//Takes in the mesh info and makes it renderable
FBXParser::FBXParser()
{
}

FBXParser::~FBXParser()
{
}

//Takes the data from the MainHeader struct that is filled and parses it into a mesh renderable on screen
void FBXParser::createMesh(Mesh& mesh)
{
	FBXBinReader meshreader;
	meshreader.binaryMeshReading();
}
