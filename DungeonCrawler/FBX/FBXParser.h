#pragma once
#include "HeaderStruct.h"
#include "FBXBinReader.h"
#include "../Graphics/MeshMap.h"

class FBXParser{

public:
	FBXParser();
	~FBXParser();


	void createMesh(Mesh& mesh);

private:

};