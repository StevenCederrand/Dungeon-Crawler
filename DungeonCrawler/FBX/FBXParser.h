#pragma once
#include "FBXParserLibrary.h"


class FBXParser
{
public:
	FBXParser();
	~FBXParser();

	FBXParserData* binaryMeshReading(std::string nameOfFile);


private:

};