#include "FBXImporter.h"

//Lib file that interprets binary values to base 10 values and returns them
namespace FBXImporter {

	//Debug function to test the .lib file and write the mesh name
	void displayMeshName()
	{
		std::cout << "Lib file working!" << std::endl;
		std::cout << "Mesh name: " << "" << std::endl;
	}

	//Debug function to test the .lib file and write the mesh vertice amount
	void displayVertices()
	{
		std::cout << "Amount of vertices:" << "" << std::endl;

	}

	//Function that takes in the path to the .bin file and reads it and return the binary data
	std::vector<unsigned int> getBinaryData(std::string pathToMesh)
	{
		std::vector<unsigned int> binaryData;
		

		std::ifstream infileBinary;
		infileBinary.open(pathToMesh, std::ios::binary | std::ios::in);

		if (infileBinary.is_open() == false)
		{
			std::cout << "Could not open the custom binary file" << std::endl;
		}

		else if (infileBinary.is_open())
		{
			//Length of file
			infileBinary.seekg(0, infileBinary.end);
			int size = infileBinary.tellg();
			infileBinary.seekg(0, infileBinary.beg);

			//Memory allocation
			char* binaryBuffer = new char[size];
			
			//Read the data one index at a time
			infileBinary.read(binaryBuffer, size);
			for (size_t i = 0; i < size; i++)
			{
				binaryData = (std::vector<unsigned int>)binaryBuffer[i];
				std::cout << binaryBuffer[i] << std::endl;
			}

			FBXImporter::displayMeshName();
			FBXImporter::displayVertices();

			std::cin.get();
		}

		return binaryData;
	}
}
