//With the .lib file included the fbxbinreader should interpret the binary data and set the data to the structs and return the data
#include "FBXBinReader.h"

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
	std::string pathtoMesh = "ourFileBinary.bin";
	FBXParserData* fileData = nullptr;
	FBXImporter::readAndWriteBinaryData(pathtoMesh, fileData);

	//std::cout << binaryFileData << std::endl;
	
}
