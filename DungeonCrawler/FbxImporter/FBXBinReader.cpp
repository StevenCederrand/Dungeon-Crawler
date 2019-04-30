//With the .lib file included the fbxbinreader should interpret the binary data and set the data to the structs and return the data
#include "FBXBinReader.h"

int main()
{
	std::ifstream::pos_type size;
	char x;
	std::ifstream infileBinary;
	infileBinary.open("ourFileBinary.bin", std::ios::binary | std::ios::in);
	
	if (infileBinary.is_open() == false)
	{
		std::cout << "Could not open the custom binary file" << std::endl;
	}

	else if (infileBinary.is_open())
	{
		size = infileBinary.tellg();
		infileBinary.read(&x, 7);

		FBXImporter::displayMeshName();
		FBXImporter::displayVertices();
		
		std::cin.get();
	}
	
}

FBXBinReader::FBXBinReader()
{
}

FBXBinReader::~FBXBinReader()
{
}

MainHeader FBXBinReader::binaryMeshReading()
{
	return MainHeader();
}
