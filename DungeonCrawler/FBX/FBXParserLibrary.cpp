#include "FBXParserLibrary.h"

//Lib file that interprets binary values to base 10 values and returns them
namespace FBXParserLibrary {

	//Debug function to test the .lib file and write the mesh name
	void displayMeshName()
	{
		//std::cout << "Lib file working!" << std::endl;
		//std::cout << "Mesh name: " << "" << std::endl;
	}

	//Debug function to test the .lib file and write the mesh vertice amount
	void displayVertices()
	{
		//std::cout << "Amount of vertices:" << "" << std::endl;

	}

	//Function that takes in the path to the .bin file and reads it and return the binary data
	void readAndWriteBinaryData(std::string pathToMesh, FBXParserData* fileData)
	{
		//std::vector<unsigned int> binaryData;
		

		std::ifstream infileBinary;
		infileBinary.open(pathToMesh, std::ios::binary | std::ios::in);

		if (infileBinary.is_open() == false)
		{
			//std::cout << "Could not open the custom binary file" << std::endl;
		}

		else if (infileBinary.is_open())
		{
			//std::cout << "--------------------FBX Importer lib file-------------------------------" << std::endl;
			//FBXImporter::displayMeshName();
			//FBXImporter::displayVertices();
			//std::cout << std::endl;
			//std::cout << "GeometryCount: " << infileBinary.gcount() << std::endl;

			displayMainHeader(infileBinary, fileData);
			displayMeshHeader(infileBinary, fileData);
			displayVertexHeader(infileBinary, fileData);
			displayMaterialHeader(infileBinary, fileData);

			for (int i = 0; i < fileData->getMainHeader().boundingBoxCount; i++)
			{
				displayBoundingBoxHeader(infileBinary, fileData);
				displayBoundingBoxVertexHeader(infileBinary, fileData, i);
			}

			calculateMinMaxValueMesh(infileBinary, fileData);
			calculateMinMaxValueHitbox(infileBinary, fileData);

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

			//std::cin.get(); //for pausing?


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
			//std::cin.get();
		}

	}

	//First
	void displayMainHeader(std::ifstream& infileBinary, FBXParserData* fileData)
	{
		char version = binaryToChar(infileBinary);
		//std::cout << "Version: " << version << " " << std::endl;
		setVersion(fileData, version);

		char paddingOne = binaryToChar(infileBinary);
		//std::cout << "paddingOne \n";

		char paddingTwo = binaryToChar(infileBinary);
		//std::cout << "paddingTwo \n";

		char paddingThree = binaryToChar(infileBinary);
		//std::cout << "paddingThree \n";

		unsigned int dynamicMeshCount = (unsigned int)binaryToInt(infileBinary);
		//std::cout << "DynamicMeshCount: " << dynamicMeshCount << " " << std::endl;
		setDynamicMeshCount(fileData, dynamicMeshCount);

		unsigned int staticMeshCount = (unsigned int)binaryToInt(infileBinary);
		//std::cout << "StaticMeshCount: " << staticMeshCount << " " << std::endl;
		setStaticMeshCount(fileData, staticMeshCount);

		unsigned int boundingBoxCount = (unsigned int)binaryToInt(infileBinary);
		//std::cout << "BoundingBoxCount: " << boundingBoxCount << " " << std::endl;
		setBoundBoxCount(fileData, boundingBoxCount);

		unsigned int materialCount = (unsigned int)binaryToInt(infileBinary);
		//std::cout << "MaterialCount: " << materialCount << " " << std::endl << std::endl;
		setMaterialCount(fileData, materialCount);
	}

	//Second
	void displayMeshHeader(std::ifstream& infileBinary, FBXParserData* fileData)
	{
		std::vector<char> lnameOfMesh;
		for (int i = 0; i < 100; i++)
		{
			lnameOfMesh.push_back(binaryToChar(infileBinary));
		}

		//std::cout << "Name of mesh: ";
		for (int i = 0; i < 100; i++)
		{
			//std::cout << lnameOfMesh[i];
		}
		//std::cout << "\n";
		setNameOfMesh(fileData, lnameOfMesh);

		unsigned int vertexCount = (unsigned int)binaryToInt(infileBinary);
		//std::cout << "VertexCount: " << vertexCount << " " << std::endl;
		setVertexCountOfMesh(fileData, vertexCount);

		int materialID = (int)binaryToInt(infileBinary);
		//std::cout << "Material ID: " << materialID << " " << std::endl;
		setMaterialIDForMesh(fileData, materialID);

		bool collision = binaryToBool(infileBinary);
		//std::cout << "Collision: "<< collision << " " << std::endl;
		setCollisionOfMesh(fileData, collision);

		bool staticMesh = binaryToBool(infileBinary);
		//std::cout << "StaticMesh: " << staticMesh << " " << std::endl;
		setStaticMesh(fileData, staticMesh);

		bool paddingOne = binaryToBool(infileBinary);
		//std::cout << "PaddingOne: " << paddingOne << " " << std::endl;

		bool paddingTwo = binaryToBool(infileBinary);
		//std::cout << "PaddingTwo: " << paddingTwo << std::endl;
	}

	//third
	void displayBoundingBoxHeader(std::ifstream& infileBinary, FBXParserData* fileData)
	{
		//Makes the header then gives it to parserdata
		BoundingBoxHeader lboundingBoxHeader;

		std::vector<char> lnameOfBoundingBox;
		for (int i = 0; i < 100; i++)
		{
			lboundingBoxHeader.nameOfHitbox[i] = (binaryToChar(infileBinary)); //Reads file
		}

		unsigned int vertexCount = (unsigned int)binaryToInt(infileBinary);
		lboundingBoxHeader.vertexCount = vertexCount;

		bool collision = binaryToBool(infileBinary);
		lboundingBoxHeader.collision = collision;

		bool staticMesh = binaryToBool(infileBinary);
		lboundingBoxHeader.staticMesh = staticMesh;

		bool paddingOne = binaryToBool(infileBinary);
		lboundingBoxHeader.padding1 = paddingOne;

		bool paddingTwo = binaryToBool(infileBinary);
		lboundingBoxHeader.padding2 = paddingTwo;

		fileData->addBoundingBoxHeader(lboundingBoxHeader);
	}

	//fourth
	void displayVertexHeader(std::ifstream& infileBinary, FBXParserData* fileData)
	{
		//Need the static mesh count from the mesh header
		for (int i = 0; i < fileData->getMeshHeader().vertexCount; i++)
		{
			float positionX = binaryToFloat(infileBinary);
			//std::cout << "Position: " << positionX << " ";

			float positionY = binaryToFloat(infileBinary);
			//std::cout << positionY << " ";

			float positionZ = binaryToFloat(infileBinary);
			//std::cout << positionZ << "\n";

			glm::vec3 vertexPos;
			vertexPos.x = positionX;
			vertexPos.y = positionY;
			vertexPos.z = positionZ;
			fileData->addVertexPos(vertexPos);

			float positionU = binaryToFloat(infileBinary);
			//std::cout << "UV: " << positionU << " ";

			float positionV = binaryToFloat(infileBinary);
			//std::cout << positionV << "\n";

			glm::vec2 vertexUV;
			vertexUV.x = positionU;
			vertexUV.y = positionV;
			fileData->addUV(vertexUV);

			float positionNX = binaryToFloat(infileBinary);
			//std::cout << "normal: " << positionNX << " ";

			float positionNY = binaryToFloat(infileBinary);
			//std::cout << positionNY << " ";

			float positionNZ = binaryToFloat(infileBinary);
			//std::cout << positionNZ << "\n";

			glm::vec3 vertexNormal;
			vertexNormal.x = positionNX;
			vertexNormal.y = positionNY;
			vertexNormal.z = positionNZ;
			fileData->addNormal(vertexNormal);
		}
	}

	void displayMaterialHeader(std::ifstream& infileBinary, FBXParserData* fileData)
	{
		std::vector<char> lnameOfAlbedoMapCHARS;
		std::vector<char> lnameOfNormalMapCHARS;

		for (int i = 0; i < 100; i++)
		{
			char currentChar = binaryToChar(infileBinary);
			lnameOfAlbedoMapCHARS.push_back(currentChar); //Reads file
		}
		fileData->setMaterialHeaderNameOfAlbedo(lnameOfAlbedoMapCHARS); //sets the name directly in ParserData

		for (int i = 0; i < 100; i++)
		{
			char currentChar = binaryToChar(infileBinary);
			lnameOfNormalMapCHARS.push_back(currentChar); //Reads file
		}
		fileData->setMaterialHeaderNameOfNormal(lnameOfNormalMapCHARS);

		int materialID = (int)binaryToInt(infileBinary);
		fileData->setMaterialHeaderMaterialID(materialID);

		unsigned int nrOfTextures = (int)binaryToInt(infileBinary);
		fileData->setMaterialHeaderNrOfTextures(nrOfTextures);

		unsigned int whatShader = (int)binaryToInt(infileBinary);
		fileData->setMaterialHeaderWhatShader(whatShader);
	}
	

	//fifth
	void displayBoundingBoxVertexHeader(std::ifstream& infileBinary, FBXParserData* fileData, int i)
	{
		//Need the bounding box mesh count from the mesh header
		for (int j = 0; j < fileData->getBoundingBoxHeaders()[i].vertexCount; j++)
		{
			float positionX = binaryToFloat(infileBinary);
			//std::cout << "Position: " << positionX << " ";

			float positionY = binaryToFloat(infileBinary);
			//std::cout << positionY << " ";

			float positionZ = binaryToFloat(infileBinary);
			//std::cout << positionZ << "\n";

			glm::vec3 vertexPos;
			vertexPos.x = positionX;
			vertexPos.y = positionY;
			vertexPos.z = positionZ;
			fileData->addVertexPos(vertexPos);
		}
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

	//--------------------------------------------------------------------------
	//------------------------------MainHeader----------------------------------
	void setVersion(FBXParserData* fileData, char versionNr)
	{
		fileData->setMainHeaderVersion(versionNr);
	}

	void setDynamicMeshCount(FBXParserData* fileData, unsigned int dynamicMeshCount)
	{
		fileData->setMainHeaderDynamicMeshCount(dynamicMeshCount);
	}

	void setStaticMeshCount(FBXParserData* fileData, unsigned int nrOfMeshes)
	{
		fileData->setMainHeaderStaticMeshCount(nrOfMeshes);
	}

	void setBoundBoxCount(FBXParserData* fileData, unsigned int nrOfBoundingBoxMeshes)
	{
		fileData->setMainHeaderBoundBoxCount(nrOfBoundingBoxMeshes);
	}

	void setMaterialCount(FBXParserData* fileData, unsigned int nrOfMaterials)
	{
		fileData->setMainHeaderMaterialCount(nrOfMaterials);
	}

	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	//------------------------------Mesh Header----------------------------------
	void setNameOfMesh(FBXParserData* fileData, std::vector<char> nameOfMeshVector)
	{
		fileData->setMeshHeaderNameOfMesh(nameOfMeshVector);
	}

	void setVertexCountOfMesh(FBXParserData* fileData, unsigned int vertexCount)
	{
		fileData->setMeshHeaderVertexCountOfMesh(vertexCount);
	}

	void setMaterialIDForMesh(FBXParserData* fileData, int materialID)
	{
		fileData->setMeshHeaderMaterialID(materialID);
	}

	void setCollisionOfMesh(FBXParserData* fileData, bool collision)
	{
		fileData->setMeshHeaderCollisionOfMesh(collision);
	}

	void setStaticMesh(FBXParserData* fileData, bool staticMesh)
	{
		fileData->setMeshHeaderStaticMesh(staticMesh);
	}
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//--------------------------Bounding Box Header-------------------------------
	void setNameOfBoundingBoxMesh(FBXParserData* fileData, char nameOfBoundingBox[])
	{
		//fileData->setBBHeaderNameOfBoundingBoxMesh(nameOfBoundingBox);
	}

	/*
	void setVertexCountOfBoundingBoxMesh(FBXParserData* fileData, unsigned int vertexCount)
	{
		fileData->setBBHeaderVertexCountOfBoundingBoxMesh(vertexCount);
	}

	void setCollisionOfBoundingBoxMesh(FBXParserData* fileData, bool collision)
	{
		fileData->setBBHeaderCollisionOfBoundingBoxMesh(collision);
	}

	void setStaticBoundingBoxMesh(FBXParserData* fileData, bool staticBoundingBoxMesh)
	{
		fileData->setBBHeaderStaticBoundingBoxMesh(staticBoundingBoxMesh);
	}
	*/
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	//----------------------------Mesh Vertex Header-------------------------------
	void setPositionVertexOfMesh(FBXParserData* fileData, float positionVertex[])
	{
		//fileData->setMVHeaderPositionVertexOfMesh(positionVertex);
	}

	void setUVOfMesh(FBXParserData* fileData, float UV[])
	{
		//fileData->setMVHeaderUVOfMesh(UV);
	}

	void setNormalOfMesh(FBXParserData* fileData, float normal[])
	{
		//fileData->setMVHeaderNormalOfMesh(normal);
	}


	//material header

	void setNameOfAlbedo(FBXParserData* fileData, std::vector<char> nameOfAlbedo)
	{
		fileData->setMaterialHeaderNameOfAlbedo(nameOfAlbedo);
	}

	void setNameOfNormal(FBXParserData* fileData, std::vector<char> nameOfNormal)
	{
		fileData->setMaterialHeaderNameOfNormal(nameOfNormal);
	}

	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	//---------------------------Bounding Box Vertex--------------------------------
	void setPositionOfBoundingBox(FBXParserData* fileData, float normal[])
	{
		fileData->setBBVHeaderPositionOfBoundingBox(normal);
	}

	//------------------------------------------------------------------------------

	void calculateMinMaxValueMesh(std::ifstream& binaryFile, FBXParserData* fileData)
	{
		//is onyl 36 now, should be 72
		std::vector<glm::vec3> allVertices = fileData->getVertexPos();
		float xMin = 0.f;
		float xMax = 0.f;
		float yMin = 0.f;
		float yMax = 0.f;

		for (int i = 0; i < fileData->getMeshHeader().vertexCount; i++)
		{
			glm::vec3 meshVertice = allVertices[i];

			if (i == 1) //first pass to make eveything compare to the first vertice
			{
				float xMin = meshVertice.x;
				float xMax = meshVertice.x;
				float yMin = meshVertice.y;
				float yMax = meshVertice.y;
			}

			if (meshVertice.x >= xMax)
				xMax = meshVertice.x;
			if (meshVertice.y >= yMax)
				yMax = meshVertice.y;

			if (meshVertice.x <= xMin)
				xMin = meshVertice.x;
			if (meshVertice.y <= yMin)
				yMin = meshVertice.y;
		}

		glm::vec4 lminMaxValues;//MINMINMAXMAX, ITS IN THE WRONG ORDER, IT SHOULD BE MIN MAX
		lminMaxValues.x = xMin;
		lminMaxValues.y = yMin;
		lminMaxValues.z = xMax;
		lminMaxValues.w = yMax;

		fileData->setMinMaxValuesMesh(lminMaxValues);

		//OBJECT SPAWN ON THE SIDE of the hitbox, the object mesh is offset by half?
	}

	void calculateMinMaxValueHitbox(std::ifstream& binaryFile, FBXParserData* fileData)
	{
		int nrOfMeshVertices = fileData->getMeshHeader().vertexCount; //should be several meshes later
		int nrOfHitboxVertices = 0;
		int nrOfHitboxVerticesLastCykle = 0;

		for (int i = 0; i < fileData->getMainHeader().boundingBoxCount; i++)
		{
			nrOfHitboxVertices += fileData->getBoundingBoxHeaders()[i].vertexCount; //fix

			std::vector<glm::vec3> allVertices = fileData->getVertexPos();
			float xMin = 0.f;
			float xMax = 0.f;
			float yMin = 0.f;
			float yMax = 0.f;
			float zMin = 0.f;
			float zMax = 0.f;


			for (int j = (nrOfMeshVertices + nrOfHitboxVerticesLastCykle); j < (nrOfMeshVertices + nrOfHitboxVertices); j++)
			{
				//XYZ MAX XYZ MIN
				glm::vec3 hitboxVertice = allVertices[j];

				if (j == (nrOfMeshVertices + nrOfHitboxVerticesLastCykle)) //first pass to make eveything compare to the first vertice
				{
					xMin = hitboxVertice.x;
					xMax = hitboxVertice.x;
					yMin = hitboxVertice.y;
					yMax = hitboxVertice.y;
					zMin = hitboxVertice.z;
					zMax = hitboxVertice.z;
				}

				if (hitboxVertice.x >= xMax)
					xMax = hitboxVertice.x;
				if (hitboxVertice.y >= yMax)
					yMax = hitboxVertice.y;
				if (hitboxVertice.z >= zMax)
					zMax = hitboxVertice.z;

				if (hitboxVertice.x <= xMin)
					xMin = hitboxVertice.x;
				if (hitboxVertice.y <= yMin)
					yMin = hitboxVertice.y;
				if (hitboxVertice.z <= zMin)
					zMin = hitboxVertice.z;
			}
			nrOfHitboxVerticesLastCykle += fileData->getBoundingBoxHeaders()[i].vertexCount;


			glm::vec3 lminValues;	//XYZ MIN
			lminValues.x = xMin;
			lminValues.y = yMin;
			lminValues.z = zMin;
			fileData->addMinMaxValuesHitbox(lminValues); 

			glm::vec3 lmaxValues;	//XYZ MAX
			lmaxValues.x = xMax;
			lmaxValues.y = yMax;
			lmaxValues.z = zMax;
			fileData->addMinMaxValuesHitbox(lmaxValues);
			
			//ALL MIN MAX VALUES ARE THE SAME, CHECK READABLE FILE
		}
	}
}
