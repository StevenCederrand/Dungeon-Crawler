//With the .lib file included the fbxbinreader should interpret the binary data and set the data to the structs and return the data
#include "FBXBinReader.h"


//HAS MAIN, SHOULDNT BE THIS WAY!!, where to call?

int main()
{
	FBXBinReader binaryData;
	binaryData.binaryMeshReading();
}

FBXBinReader::FBXBinReader()
{
}

FBXBinReader::~FBXBinReader()
{
}

void FBXBinReader::binaryMeshReading()
{
	//Decide what file to open with a string
	std::string pathtoMesh = "ourFileBinary.bin";
	
	//Allocate memory for the FBXParserData class and assign it to a pointer type
	FBXParserData* fileData = new FBXParserData;

	//Go into the lib files namespace FBXImporter and send in the string to read and the FBXParserData to retrieve
	FBXImporter::readAndWriteBinaryData(pathtoMesh, fileData);

	std::cout << "-----------------------------FBX Bin Reader-----------------------------" << std::endl;
	std::cout << "Version: " << fileData->getMainHeader().version << std::endl;
	std::cout << "Dynamic mesh count: "<< fileData->getMainHeader().dynamicMeshCount << std::endl;
	std::cout << "Static mesh count: " << fileData->getMainHeader().staticMeshCount << std::endl;
	std::cout << "Bounding box mesh count: " << fileData->getMainHeader().boundingBoxCount << std::endl;
	
	std::cout << "Mesh vertex count: " << fileData->getMeshHeader().vertexCount << std::endl;
	std::cout << "Mesh collision: " << fileData->getMeshHeader().collision << std::endl;
	std::cout << "Mesh static mesh: " << fileData->getMeshHeader().staticMesh << std::endl;

	std::cout << "Bounding box Vertex Count: " << fileData->getBoundingBoxHeader().vertexCount << std::endl;
	std::cout << "Bounding box collision: " << fileData->getBoundingBoxHeader().collision << std::endl;
	std::cout << "Bounding box static Mesh: " << fileData->getBoundingBoxHeader().staticMesh << std::endl;
	
	std::cout << "Mesh vertex position: " << fileData->getVertexHeader().position << std::endl;
	std::cout << "Mesh uv position: " << fileData->getVertexHeader().UV << std::endl;
	std::cout << "Mesh normal position: " << fileData->getVertexHeader().normal << std::endl;

	std::cout << "Bounding box position: " << fileData->getBoundingBoxVertexHeader().position << std::endl;

	std::cin.get();
	//std::cout << binaryFileData << std::endl;
	
}
