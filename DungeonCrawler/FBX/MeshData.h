#pragma once

#include "DisplayCommon.h"

class MeshData
{
private:
	int m_nrOfControlPoints;
	FbxVector4* m_lControlPoints[];
public:
	MeshData();
};