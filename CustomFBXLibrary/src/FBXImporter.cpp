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
			
			//structured in the way that the custom binary file is written
			int binaryData = binaryToInt(infileBinary);
			setStaticMeshCount(fileData, binaryData);
			std::cout << fileData->getMainHeaderStaticMeshCount();
			binaryToFloat(infileBinary);
			std::cin.get();


			////Length of file
			//infileBinary.seekg(0, infileBinary.end);
			//int size = infileBinary.tellg();
			//infileBinary.seekg(0, infileBinary.beg);
			////Memory allocation
			//char* binaryBuffer = new char[size + 1];
			////Read the binary data in one batch
			//infileBinary.read(binaryBuffer, size);
	
			//
			////Print the binary data in ascii format one index at a time
			//int j = 0;
			//for (size_t i = 0; i < size; i++)
			//{
			//	std::cout << std::hex  << binaryBuffer[i] << " ";
			//	j++;

			//	//Separates the output with a new line
			//	if (j > 15)
			//	{
			//		std::cout << std::hex << std::endl;
			//		j = 0;
			//	}
			//}


			std::cin.get();
		}

	}

	int binaryToInt(std::ifstream& binaryFile)
	{
		//char array to string
		//string base 2 to base 10
		char binaryNumber[] = "11111110";
		int convertedNumber = std::stoi(binaryNumber, nullptr, 2);
		std::cout << "test 1: " << convertedNumber;

		return convertedNumber;
	}

	float binaryToFloat(std::ifstream& binaryFile)
	{
		char binaryNumber[] = "1111111011111110";
		float convertedNumber =std::stof(binaryNumber, nullptr);
		std::cout << "test 1: " << convertedNumber;

		return convertedNumber;
	}

	void setStaticMeshCount(FBXParserData* fbxParserData, int nrOfMeshes)
	{
		fbxParserData->setMainHeaderStaticMeshCount(nrOfMeshes);
	}


}
