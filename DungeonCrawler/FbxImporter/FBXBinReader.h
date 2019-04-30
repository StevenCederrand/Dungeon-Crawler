#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "FBXImporter.h"
#include "HeaderStruct.h"


class FBXBinReader
{
public:
	FBXBinReader();
	~FBXBinReader();

	MainHeader binaryMeshReading();


private:

};