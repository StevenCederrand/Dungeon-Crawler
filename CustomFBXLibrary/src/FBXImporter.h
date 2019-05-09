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
	
	void displayMainHeader(std::ifstream& binaryFile, FBXParserData* filedata);
	void displayMeshHeader(std::ifstream& binaryFile, FBXParserData* fileData);
	void displayBoundingBoxHeader(std::ifstream& binaryFile, FBXParserData* fileData);

	void displayVertexHeader(std::ifstream& binaryFile, FBXParserData* fileData);
	void displayBoundingBoxVertexHeader(std::ifstream& binaryFile, FBXParserData* fileData);

	int binaryToInt(std::ifstream& binaryFile);
	float binaryToFloat(std::ifstream& binaryFile);
	bool binaryToBool(std::ifstream& binaryFile);
	char binaryToChar(std::ifstream& binaryFile);


	//Struct set functions in order
	//MainHeader
	void setVersion(FBXParserData* fileData, char versionNr);
	void setDynamicMeshCount(FBXParserData* fileData, unsigned int dynamicMeshCount);
	void setStaticMeshCount(FBXParserData* fileData, unsigned int nrOfMeshes);
	void setBoundBoxCount(FBXParserData* fileData, unsigned int nrOfBoundingBoxMeshes);

	//MeshHeader
	void setNameOfMesh(FBXParserData* fileData, char nameOfMesh[]);
	void setVertexCountOfMesh(FBXParserData* fileData, unsigned int vertexCount);
	void setCollisionOfMesh(FBXParserData* fileData, bool collision);
	void setStaticMesh(FBXParserData* fileData, bool staticMesh);

	//BoundingBoxHeader
	void setNameOfBoundingBoxMesh(FBXParserData* fileData, char nameOfBoundingBox[]);
	void setVertexCountOfBoundingBoxMesh(FBXParserData* fileData, unsigned int vertexCount);
	void setCollisionOfBoundingBoxMesh(FBXParserData* fileData, bool collision);
	void setStaticBoundingBoxMesh(FBXParserData* fileData, bool staticBoundingBoxMesh);

	//Vertex
	void setPositionVertexOfMesh(FBXParserData* fileData, float positionVertex[]);
	void setUVOfMesh(FBXParserData* fileData, float UV[]);
	void setNormalOfMesh(FBXParserData* fileData, float normal[]);

	//BoundingBoxVertex
	void setPositionOfBoundingBox(FBXParserData* fileData, float normal[]);
}