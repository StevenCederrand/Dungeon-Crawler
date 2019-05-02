#pragma once
#include "Headerstruct.h"
class FBXParserData {
public:

	void setMainHeader(MainHeader mainHeader);
	void setMeshHeader(MeshHeader meshHeader);
	void setVertexHeader(Vertex vertexHeader);

	MainHeader getMainHeader()const;
	MeshHeader getMeshHeader()const;
	Vertex getVertexHeader()const;

private:
	MainHeader m_mainHeader;
	MeshHeader m_meshHeader;
	Vertex m_vertexHeader;

};