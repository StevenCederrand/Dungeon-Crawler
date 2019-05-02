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
	void readAndWriteBinaryData(std::string pathToMesh, FBXParserData* fileData)
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

			FBXImporter::displayMeshName();
			FBXImporter::displayVertices();

			std::cout << std::endl;
			
			binaryToInt(infileBinary, fileData);



			//Length of file
			infileBinary.seekg(0, infileBinary.end);
			int size = infileBinary.tellg();
			infileBinary.seekg(0, infileBinary.beg);
			//Memory allocation
			char* binaryBuffer = new char[size + 1];
			//Read the binary data in one batch
			infileBinary.read(binaryBuffer, size);
	
			
			//Print the binary data in ascii format one index at a time
			int j = 0;
			for (size_t i = 0; i < size; i++)
			{
				std::cout << std::hex  << binaryBuffer[i] << " ";
				j++;

				//Separates the output with a new line
				if (j > 15)
				{
					std::cout << std::hex << std::endl;
					j = 0;
				}
			}


			std::cin.get();
		}

	}

	void binaryToInt(std::ifstream& binaryFile, FBXParserData* fbxParserData)
	{
		
	}
}
