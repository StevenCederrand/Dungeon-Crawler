#include "FBXParserLibrary.h"

namespace FBXParserLibrary 
{
	//Function that takes in the path to the .bin file and reads it and return the binary data
	void readAndWriteBinaryData(std::string pathToMesh, FBXParserData* fileData)
	{
		std::ifstream infileBinary;
		infileBinary.open(pathToMesh, std::ios::binary | std::ios::in);

		if (infileBinary.is_open() == false)
		{
			std::cout << "Could not open the custom binary file" << std::endl;
		}
		else if (infileBinary.is_open())
		{
			saveMainHeader(infileBinary, fileData);

			for (int i = 0; i < fileData->getMainHeader().staticMeshCount; i++)
			{
				saveMeshHeader(infileBinary, fileData);
				saveVertexHeader(infileBinary, fileData, i);
				calculateMinMaxValueMesh(infileBinary, fileData, i);
			}

			saveMaterialHeader(infileBinary, fileData);

			for (int i = 0; i < fileData->getMainHeader().boundingBoxCount; i++)
			{
				saveBoundingBoxHeader(infileBinary, fileData);
				saveBoundingBoxVertexHeader(infileBinary, fileData, i);
			}

			
			calculateMinMaxValueHitbox(infileBinary, fileData);

			infileBinary.close();
		}
	}

	void saveMainHeader(std::ifstream& infileBinary, FBXParserData* fileData)
	{
		MainHeader lmainHeader;

		char version = binaryToChar(infileBinary);
		lmainHeader.version = version;

		char paddingOne = binaryToChar(infileBinary);
		lmainHeader.padding1 = paddingOne;
		char paddingTwo = binaryToChar(infileBinary);
		lmainHeader.padding2 = paddingTwo;
		char paddingThree = binaryToChar(infileBinary);
		lmainHeader.padding3 = paddingThree;

		unsigned int dynamicMeshCount = (unsigned int)binaryToInt(infileBinary);
		lmainHeader.dynamicMeshCount = dynamicMeshCount;

		unsigned int staticMeshCount = (unsigned int)binaryToInt(infileBinary);
		lmainHeader.staticMeshCount = staticMeshCount;

		unsigned int boundingBoxCount = (unsigned int)binaryToInt(infileBinary);
		lmainHeader.boundingBoxCount = boundingBoxCount;

		unsigned int materialCount = (unsigned int)binaryToInt(infileBinary);
		lmainHeader.materialCount = materialCount;

		fileData->setMainHeader(lmainHeader);
	}

	void saveMeshHeader(std::ifstream& infileBinary, FBXParserData* fileData)
	{
		MeshHeader lmeshHeader;

		std::vector<char> lnameOfMesh;
		for (int i = 0; i < 100; i++)
		{
			lnameOfMesh.push_back(binaryToChar(infileBinary));
			lmeshHeader.nameOfMesh[i] = lnameOfMesh[i];
		}

		unsigned int vertexCount = (unsigned int)binaryToInt(infileBinary);
		lmeshHeader.vertexCount = vertexCount;

		int materialID = (int)binaryToInt(infileBinary);
		lmeshHeader.materialID = materialID;

		bool collision = binaryToBool(infileBinary);
		lmeshHeader.collision = collision;

		bool staticMesh = binaryToBool(infileBinary);
		lmeshHeader.staticMesh = staticMesh;

		bool paddingOne = binaryToBool(infileBinary);
		lmeshHeader.padding1 = paddingOne;
		bool paddingTwo = binaryToBool(infileBinary);
		lmeshHeader.padding2 = paddingTwo;

		fileData->addMeshHeader(lmeshHeader); //lmesh header should be vector of mesheaders so add function here
		int stop = 1;
	}

	void saveBoundingBoxHeader(std::ifstream& infileBinary, FBXParserData* fileData)
	{
		BoundingBoxHeader lboundingBoxHeader;

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

	void saveVertexHeader(std::ifstream& infileBinary, FBXParserData* fileData, int vectorNr)
	{
		//Need the static mesh count from the mesh header
		for (int i = 0; i < fileData->getMeshHeaders()[vectorNr].vertexCount; i++) //change from 0
		{
			float positionX = binaryToFloat(infileBinary);
			float positionY = binaryToFloat(infileBinary);
			float positionZ = binaryToFloat(infileBinary);

			glm::vec3 vertexPos;
			vertexPos.x = positionX;
			vertexPos.y = positionY;
			vertexPos.z = positionZ;

			fileData->addVertexPos(vertexPos);

			float positionU = binaryToFloat(infileBinary);
			float positionV = binaryToFloat(infileBinary);

			glm::vec2 vertexUV;
			vertexUV.x = positionU;
			vertexUV.y = positionV;

			fileData->addUV(vertexUV);

			float positionNX = binaryToFloat(infileBinary);
			float positionNY = binaryToFloat(infileBinary);
			float positionNZ = binaryToFloat(infileBinary);

			glm::vec3 vertexNormal;
			vertexNormal.x = positionNX;
			vertexNormal.y = positionNY;
			vertexNormal.z = positionNZ;

			fileData->addNormal(vertexNormal);
		}
		fileData->moveVerticePosToVector();
	}

	void saveMaterialHeader(std::ifstream& infileBinary, FBXParserData* fileData)
	{
		Material lmaterialHeader;

		std::vector<char> lnameOfAlbedoMapCHARS;
		std::vector<char> lnameOfNormalMapCHARS;

		for (int i = 0; i < 100; i++)
		{
			char currentChar = binaryToChar(infileBinary);
			lnameOfAlbedoMapCHARS.push_back(currentChar); //Reads file

			lmaterialHeader.nameOfAlbedo[i] = currentChar;
		}

		for (int i = 0; i < 100; i++)
		{
			char currentChar = binaryToChar(infileBinary);
			lnameOfNormalMapCHARS.push_back(currentChar); //Reads file

			lmaterialHeader.nameOfNormal[i] = currentChar;
		}

		int materialID = (int)binaryToInt(infileBinary);
		lmaterialHeader.materialID = materialID;

		unsigned int nrOfTextures = (int)binaryToInt(infileBinary);
		lmaterialHeader.nrOfTextures = nrOfTextures;

		unsigned int whatShader = (int)binaryToInt(infileBinary);
		lmaterialHeader.whatShader = whatShader;

		fileData->setMaterialHeader(lmaterialHeader);
	}
	
	void saveBoundingBoxVertexHeader(std::ifstream& infileBinary, FBXParserData* fileData, int vectorNr)
	{
		//Need the bounding box mesh count from the mesh header
		for (int j = 0; j < fileData->getBoundingBoxHeaders()[vectorNr].vertexCount; j++)
		{
			float positionX = binaryToFloat(infileBinary);
			float positionY = binaryToFloat(infileBinary);
			float positionZ = binaryToFloat(infileBinary);

			glm::vec3 vertexPos;
			vertexPos.x = positionX;
			vertexPos.y = positionY;
			vertexPos.z = positionZ;
			fileData->addVertexPosHitbox(vertexPos);
		}
	}

	int binaryToInt(std::ifstream& binaryFile)
	{
		int convertedIntNumber;
		binaryFile.read((char*)& convertedIntNumber, sizeof(int));
	
		return convertedIntNumber;
	}

	float binaryToFloat(std::ifstream& binaryFile)
	{
		float convertedFloatNumber;
		binaryFile.read((char*)& convertedFloatNumber, sizeof(float));

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
		char convertedCharacter;
		binaryFile.read((char*)& convertedCharacter, sizeof(char));

		return convertedCharacter;
	}

	void calculateMinMaxValueMesh(std::ifstream& binaryFile, FBXParserData* fileData, int vectorNr)
	{
		//is onyl 36 now, should be 72
		std::vector<glm::vec3> currentMeshVertices = fileData->getVerticePosVector()[vectorNr];
		float xMin = 0.f;
		float xMax = 0.f;
		float yMin = 0.f;
		float yMax = 0.f;

		for (int i = 0; i < fileData->getMeshHeaders()[vectorNr].vertexCount; i++)	//change from 0
		{
			glm::vec3 meshVertice = currentMeshVertices[i];

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

		glm::vec4 lminMaxValues;//MINMINMAXMAX
		lminMaxValues.x = xMin;
		lminMaxValues.y = yMin;
		lminMaxValues.z = xMax;
		lminMaxValues.w = yMax;

		fileData->setMinMaxValuesMesh(lminMaxValues);
	}

	void calculateMinMaxValueHitbox(std::ifstream& binaryFile, FBXParserData* fileData)
	{
		int nrOfHitboxVertices = 0;
		int nrOfHitboxVerticesLastCykle = 0;

		for (int i = 0; i < fileData->getMainHeader().boundingBoxCount; i++)
		{
			nrOfHitboxVertices += fileData->getBoundingBoxHeaders()[i].vertexCount;

			std::vector<glm::vec3> hitboxVertices = fileData->getVertexPosHitbox();
			float xMin = 0.f;
			float xMax = 0.f;
			float yMin = 0.f;
			float yMax = 0.f;
			float zMin = 0.f;
			float zMax = 0.f;


			for (int j = nrOfHitboxVerticesLastCykle; j < nrOfHitboxVertices; j++)
			{
				//XYZ MAX XYZ MIN
				glm::vec3 hitboxVertice = hitboxVertices[j];

				if (j == nrOfHitboxVerticesLastCykle) //first pass to make eveything compare to the first vertice
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
		}
	}
}
