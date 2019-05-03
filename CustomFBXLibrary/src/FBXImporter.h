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

	int binaryToInt(std::ifstream& binaryFile);
	float binaryToFloat(std::ifstream& binaryFile);

	void setStaticMeshCount(FBXParserData* fbxParserData, int nrOfMeshes);
}