#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include "FBXParserData.h"

namespace FBXImporter {
	void displayMeshName();
	void displayVertices();


	void readAndWriteBinaryData(std::string pathToMesh, FBXParserData* fileData);
	
	void displayMainHeader(std::ifstream& binaryFile);
	void displayMeshHeader(std::ifstream& binaryFile);
	void displayVertexHeader(std::ifstream& binaryFile);

	void displayBoundingBoxHeader(std::ifstream& binaryFile);
	void displayBoundingBoxVertexHeader(std::ifstream& binaryFile);

	int binaryToInt(std::ifstream& binaryFile);
	float binaryToFloat(std::ifstream& binaryFile);
	bool binaryToBool(std::ifstream& binaryFile);
	char binaryToChar(std::ifstream& binaryFile);


	void setStaticMeshCount(FBXParserData* fbxParserData, int nrOfMeshes);

	//Temporary variables(should use the FBXParserDatas structs instead
	unsigned int m_staticMeshCount;
	unsigned int m_boundingBoxMeshCount;

	unsigned int m_staticMeshVertexCount;
	unsigned int m_boundingBoxVertexCount;
}