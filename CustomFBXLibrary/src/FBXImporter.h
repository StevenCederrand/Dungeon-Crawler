#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <fstream>
#include <iostream>
#include "FBXParserData.h"

namespace FBXImporter {
	void displayMeshName();
	void displayVertices();


	void readAndWriteBinaryData(std::string pathToMesh, FBXParserData* fileData);

	void binaryToInt(std::ifstream& binaryFile, FBXParserData* fbxParserData);
}