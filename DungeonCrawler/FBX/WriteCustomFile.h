#pragma once

#define DELTA 0.0001
#define EQUAL(A,B) (abs((A)-(B)) < DELTA) ? true:false

#include <iostream>
#include <fstream>
#include <string>
#include "HeaderStructs.h"
#include "StaticMesh.h"
#include "BoundingBoxMesh.h"

class WriteCustomFile
{
private:
	void CreateCustomFile();
	MainHeader m_mainHeader;
public:
	WriteCustomFile();
	~WriteCustomFile();
	void WriteMainHeader(int nrOfStaticMeshes, int nrOfBoundingBoxes);
	void WriteStaticMesh(StaticMesh currentMesh);
	void WriteBoundingBoxMesh(BoundingBoxMesh currentMesh);

	void SmallFile(); //writes small file
};