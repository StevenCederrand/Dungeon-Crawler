//With the .lib file included the fbxbinreader should interpret the binary data and set the data to the structs and return the data
#include "FBXBinReader.h"


//HAS MAIN, SHOULDNT BE THIS WAY!!
//Has Parser data, here I add vertices?

/*
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	FBXBinReader binaryData;
	binaryData.binaryMeshReading();
}
*/


FBXBinReader::FBXBinReader()
{
}

FBXBinReader::~FBXBinReader()
{
	//Remember to delete fileData!!
}

FBXParserData* FBXBinReader::binaryMeshReading(std::string nameOfFile)
{
	//Decide what file to open with a string
	std::string pathtoMesh = nameOfFile;
	
	//Allocate memory for the FBXParserData class and assign it to a pointer type
	FBXParserData* fileData = new FBXParserData(100);

	//Go into the lib files namespace FBXImporter and send in the string to read and the FBXParserData to retrieve

	//loads the file with info to be read out here
	FBXImporter::readAndWriteBinaryData(pathtoMesh, fileData);

	


	std::cout << "-----------------------------FBX Bin Reader-----------------------------" << std::endl;
	std::cout << "MainHeader\n\n";
	std::cout << "Version: " << fileData->getMainHeader().version << std::endl;
	std::cout << "Dynamic mesh count: "<< fileData->getMainHeader().dynamicMeshCount << std::endl;
	std::cout << "Static mesh count: " << fileData->getMainHeader().staticMeshCount << std::endl;
	std::cout << "Bounding box mesh count: " << fileData->getMainHeader().boundingBoxCount << std::endl;
	std::cout << "Material count: " << fileData->getMainHeader().materialCount << "\n\n";
	
	std::string nameOfStaticMesh;
	for (int i = 0; i < 100; i++)
		nameOfStaticMesh += fileData->getMeshHeader().nameOfMesh[i];



	//SHOULD HAVE SEVERAL MESH HEADERS TO SUPPORT SEVERAL MESHES!! FOR LOOP MAIN HEADER NR OF STATIC MESHES??

	std::cout << "Name of static mesh: " << nameOfStaticMesh;
	std::cout << "Mesh vertex count: " << fileData->getMeshHeader().vertexCount << std::endl;
	std::cout << "Mesh collision: " << fileData->getMeshHeader().collision << std::endl;
	std::cout << "Mesh static mesh: " << fileData->getMeshHeader().staticMesh << std::endl;

	//all vertice data from regular mesh
	std::vector<glm::vec3> pos = fileData->getVertexPos();
	std::vector<glm::vec2> UV = fileData->getUVs();
	std::vector<glm::vec3> normal = fileData->getNormals();
	std::cout << "Vertice Pos saved in FBXParserData: \n";
	for (int i = 0; i < fileData->getMeshHeader().vertexCount; i++)
	{
		std::cout << "Position: " << pos[i].x << " " << pos[i].y << " " << pos[i].z << "\n";
		std::cout << "UV : " << UV[i].x << " " << UV[i].y << "\n";
		std::cout << "Normals: " << normal[i].x << " " << normal[i].y << " " << normal[i].z << "\n";
		std::cout << "\n";
	}

	std::vector<std::string> albedoMapName = fileData->getAlbedoMapName();
	std::vector<std::string> normalMapName = fileData->getNormalMapName();

	//ONLY SUPPORTS MESHES THAT HAVE ONE ALBEDO AND ONE NORMALMAP
	for (int i = 0; i < fileData->getMainHeader().materialCount; i++)
	{
		std::cout << "Albedo texture " << i << " " << albedoMapName[i] << "\n";
		//std::cout << "Normal texture " << i << normalMapName[i] << "\n";
	}

	std::cout << "Bounding box Vertex Count: " << fileData->getBoundingBoxHeader().vertexCount << std::endl;
	std::cout << "Bounding box collision: " << fileData->getBoundingBoxHeader().collision << std::endl;
	std::cout << "Bounding box static Mesh: " << fileData->getBoundingBoxHeader().staticMesh << std::endl;
	
	std::cout << "Mesh vertex position: " << fileData->getVertexHeader().position << std::endl;
	std::cout << "Mesh uv position: " << fileData->getVertexHeader().UV << std::endl;
	std::cout << "Mesh normal position: " << fileData->getVertexHeader().normal << std::endl;

	std::cout << "Bounding box position: " << fileData->getBoundingBoxVertexHeader().position << std::endl;

	//std::cout << binaryFileData << std::endl;
	
	return fileData;
}
