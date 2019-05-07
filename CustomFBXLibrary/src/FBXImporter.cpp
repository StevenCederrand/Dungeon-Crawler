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
		//std::vector<unsigned int> binaryData;
		

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
			std::cout << infileBinary.gcount() << std::endl;

			displayMainHeader(infileBinary);
			displayMeshHeader(infileBinary);
			displayVertexHeader(infileBinary);

			//for (int i = 0; i < m_staticMeshCount; i++)
			//{
			//	displayMeshHeader(infileBinary);
			//	displayVertexHeader(infileBinary);
			//}

			//displayBoundingBoxHeader(infileBinary);
			//----------------------------------------------
			//Custom file interpretation
			//char version = binaryToChar(infileBinary);
			//std::cout << version << " ";

			//char paddingOne = binaryToChar(infileBinary);
			//std::cout << paddingOne << " ";

			//char paddingTwo = binaryToChar(infileBinary);
			//std::cout << paddingTwo << " ";

			//char paddingThree = binaryToChar(infileBinary);
			//std::cout << paddingThree << " ";

			//unsigned int dynamicMeshCount = (unsigned int)binaryToInt(infileBinary);
			//std::cout << dynamicMeshCount << " ";

			//unsigned int staticMeshCount = (unsigned int)binaryToInt(infileBinary);
			//std::cout << staticMeshCount << " ";

			//unsigned int boundingBoxCount = (unsigned int)binaryToInt(infileBinary);
			//std::cout << boundingBoxCount << " ";

			//setStaticMeshCount(fileData, binaryDataOne);
			//std::cout << fileData->getMainHeaderStaticMeshCount();

			//float binaryDataTwo = binaryToFloat(infileBinary);
			//std::cout << binaryDataTwo << " ";

			//----------------------------------------------

			std::cin.get();


			//-----------------------------------------------
			////Small file bin test
			////structured in the way that the custom binary file is written
			//int binaryDataOne = binaryToInt(infileBinary);
			//std::cout << binaryDataOne << " ";
			////setStaticMeshCount(fileData, binaryDataOne);
			////std::cout << fileData->getMainHeaderStaticMeshCount();
			//
			//float binaryDataTwo = binaryToFloat(infileBinary);
			//std::cout << binaryDataTwo << " ";

			//char binaryDataThree = binaryToChar(infileBinary);
			//std::cout << binaryDataThree << " ";

			//char binaryDataFour = binaryToChar(infileBinary);
			//std::cout << binaryDataFour << " ";

			//char binaryDataFive = binaryToChar(infileBinary);
			//std::cout << binaryDataFive << " ";

			//std::cin.get();
			//-----------------------------------------------

			//-----------------------------------------------
			// Ascii output of binary data
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
			//-----------------------------------------------


			infileBinary.close();
			std::cin.get();
		}

	}

	//First
	void displayMainHeader(std::ifstream& infileBinary)
	{
		char version = binaryToChar(infileBinary);
		std::cout << version << " ";

		char paddingOne = binaryToChar(infileBinary);
		std::cout << paddingOne << " ";

		char paddingTwo = binaryToChar(infileBinary);
		std::cout << paddingTwo << " ";

		char paddingThree = binaryToChar(infileBinary);
		std::cout << paddingThree << " ";

		unsigned int dynamicMeshCount = (unsigned int)binaryToInt(infileBinary);
		std::cout << dynamicMeshCount << " ";

		unsigned int staticMeshCount = (unsigned int)binaryToInt(infileBinary);
		std::cout << staticMeshCount << " ";
		m_staticMeshCount = staticMeshCount;
		
		unsigned int boundingBoxCount = (unsigned int)binaryToInt(infileBinary);
		std::cout << boundingBoxCount << " " << std::endl;
		m_boundingBoxMeshCount = boundingBoxCount;
	}

	//Second
	void displayMeshHeader(std::ifstream& infileBinary)
	{
		char version;

		for (int i = 0; i < 100; i++)
		{
			version = binaryToChar(infileBinary);
			std::cout << version;
		}

		unsigned int vertexCount = (unsigned int)binaryToInt(infileBinary);
		std::cout << vertexCount << " ";
		m_staticMeshVertexCount = vertexCount;

		bool collision = binaryToBool(infileBinary);
		std::cout << collision << " ";

		bool staticMesh = binaryToBool(infileBinary);
		std::cout << staticMesh << " ";

		bool paddingOne = binaryToBool(infileBinary);
		std::cout << paddingOne << " ";

		bool paddingTwo = binaryToBool(infileBinary);
		std::cout << paddingTwo << std::endl;
	}

	//Third
	void displayVertexHeader(std::ifstream& infileBinary)
	{
		//Need the static mesh count from the mesh header
		for (int i = 0; i < m_boundingBoxVertexCount; i++)
		{
			float positionX = binaryToFloat(infileBinary);
			std::cout << positionX << " ";

			float positionY = binaryToFloat(infileBinary);
			std::cout << positionY << " ";

			float positionZ = binaryToFloat(infileBinary);
			std::cout << positionZ << " " << std::endl;
		}
	}

	//fourth
	void displayBoundingBoxHeader(std::ifstream& infileBinary)
	{
		char nameOfHitBox;

		for (int i = 0; i < 100; i++)
		{
			nameOfHitBox = binaryToChar(infileBinary);
			std::cout << nameOfHitBox;
		}

		unsigned int vertexCount = (unsigned int)binaryToInt(infileBinary);
		std::cout << vertexCount << " ";
		m_boundingBoxMeshCount = vertexCount;

		bool collision = binaryToBool(infileBinary);
		std::cout << collision << " ";

		bool staticMesh = binaryToBool(infileBinary);
		std::cout << staticMesh << " ";

		bool paddingOne = binaryToBool(infileBinary);
		std::cout << paddingOne << " ";

		bool paddingTwo = binaryToBool(infileBinary);
		std::cout << paddingTwo << std::endl;
	}

	//fifth
	void displayBoundingBoxVertexHeader(std::ifstream& infileBinary)
	{
		//Need the bounding box mesh count from the mesh header
	}

	int binaryToInt(std::ifstream& binaryFile)
	{
		//binary value to int
		int convertedIntNumber;
		binaryFile.read((char*)& convertedIntNumber, sizeof(int));
	
		return convertedIntNumber;
	}

	float binaryToFloat(std::ifstream& binaryFile)
	{
		//binary value to float
		float convertedFloatNumber;
		binaryFile.read((char*)& convertedFloatNumber, sizeof(float));

		//char binaryNumber[] = "1111111011111110";
		//float convertedNumber =std::stof(binaryNumber, nullptr);
		//std::cout << "test 1: " << convertedNumber;

		return convertedFloatNumber;
	}

	bool binaryToBool(std::ifstream& binaryFile)
	{
		//binary value to char
		bool convertedBool;
		binaryFile.read((char*)& convertedBool, sizeof(bool));

		return convertedBool;
	}

	char binaryToChar(std::ifstream& binaryFile)
	{
		//binary value to char
		char convertedCharacter;
		binaryFile.read((char*)& convertedCharacter, sizeof(char));

		return convertedCharacter;
	}

	void setStaticMeshCount(FBXParserData* fbxParserData, int nrOfMeshes)
	{
		fbxParserData->setMainHeaderStaticMeshCount(nrOfMeshes);
	}


}
