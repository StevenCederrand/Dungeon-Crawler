#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include "FBXParserData.h"

namespace FBXParserLibrary 
{
	void readAndWriteBinaryData(std::string pathToMesh, FBXParserData* fileData);
	
	void saveMainHeader(std::ifstream& binaryFile, FBXParserData* filedata);
	void saveMeshHeader(std::ifstream& binaryFile, FBXParserData* fileData);
	void saveBoundingBoxHeader(std::ifstream& binaryFile, FBXParserData* fileData);
	void saveMaterialHeader(std::ifstream& binaryFile, FBXParserData* fileData);

	void saveVertexHeader(std::ifstream& binaryFile, FBXParserData* fileData, int vectorNr);
	void saveBoundingBoxVertexHeader(std::ifstream& binaryFile, FBXParserData* fileData, int vectorNr);

	//reading datatypes
	int binaryToInt(std::ifstream& binaryFile);
	float binaryToFloat(std::ifstream& binaryFile);
	bool binaryToBool(std::ifstream& binaryFile);
	char binaryToChar(std::ifstream& binaryFile);

	void calculateMinMaxValueMesh(std::ifstream& binaryFile, FBXParserData* fileData, int vectorNr);
	void calculateMinMaxValueHitbox(std::ifstream& binaryFile, FBXParserData* fileData);
}