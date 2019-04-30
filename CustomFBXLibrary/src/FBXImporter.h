#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace FBXImporter {
	void displayMeshName();
	void displayVertices();
	//Insert vector
	

	std::vector<unsigned int> getBinaryData(std::string nameOfMesh);
}