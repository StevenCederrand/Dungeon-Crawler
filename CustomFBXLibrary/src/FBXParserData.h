#pragma once
#include "Headerstruct.h"
class FBXParserData {
public:

	void setMainHeader(MainHeader mainHeader);
	void setMeshHeader(MeshHeader meshHeader);
	void setVertexHeader(Vertex vertexHeader);

	void setMainHeaderStaticMeshCount(int staticMeshCount);

	MainHeader getMainHeader()const;
	MeshHeader getMeshHeader()const;
	Vertex getVertexHeader()const;

	int getMainHeaderStaticMeshCount()const;

private:
	MainHeader m_mainHeader;
	MeshHeader m_meshHeader;
	Vertex m_vertexHeader;

};