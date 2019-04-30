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

MainHeader FBXBinReader::binaryMeshReading()
{
	std::string pathtoMesh = "ourFileBinary.bin";
	std::vector<unsigned int> binaryFileData = FBXImporter::getBinaryData(pathtoMesh);

	//std::cout << binaryFileData << std::endl;
	return MainHeader();
}
