#pragma once

#define DELTA 0.0001
#define EQUAL(A,B) (abs((A)-(B)) < DELTA) ? true:false

#include <iostream>
#include <fstream>
#include "HeaderStructs.h"
#include "StaticMesh.h"
#include "BoundingBoxMesh.h"

class WriteCustomFile
{
private:

public:
	WriteCustomFile();
	~WriteCustomFile();
	void CreateCustomFile();
	void WriteStaticMesh(StaticMesh currentMesh);
	void WriteBoundingBoxMesh(BoundingBoxMesh currentMesh);
};