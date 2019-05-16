#pragma once
#include "FBXImporter.h"


class FBXBinReader
{
public:
	FBXBinReader();
	~FBXBinReader();

	FBXParserData* binaryMeshReading(std::string nameOfFile);


private:

};