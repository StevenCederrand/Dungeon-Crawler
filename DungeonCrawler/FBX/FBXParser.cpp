//With the .lib file included the fbxbinreader should interpret the binary data and set the data to the structs and return the data
#include "FBXParser.h"


FBXParser::FBXParser()
{

}

FBXParser::~FBXParser()
{
	//Remember to delete fileData!!
}

FBXParserData* FBXParser::binaryMeshReading(std::string nameOfFile)
{
	//Decide what file to open with a string
	std::string pathtoMesh = nameOfFile;
	
	//Allocate memory for the FBXParserData class and assign it to a pointer type
	FBXParserData* fileData = new FBXParserData(100);

	//loads the file with info to be read out here
	FBXParserLibrary::readAndWriteBinaryData(pathtoMesh, fileData);

	return fileData;
}
