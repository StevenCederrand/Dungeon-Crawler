#pragma once
#include "Headerstruct.h"

class FBXParserData {
public:

	void setMainHeader(MainHeader mainHeader);
	void setMeshHeader(MeshHeader meshHeader);
	void setVertexHeader(Vertex vertexHeader);

	//-----------------------------------------------------------------
	//Struct getters functions in order
	MainHeader getMainHeader()const;
	MeshHeader getMeshHeader()const;
	BoundingBoxHeader getBoundingBoxHeader()const;
	Vertex getVertexHeader()const;
	BoundingBoxVertex getBoundingBoxVertexHeader()const;

	//-----------------------------------------------------------------


	//-----------------------------------------------------------------
	//Struct setters functions in order
	//MainHeader
	void setMainHeaderVersion(char versionNr);
	void setMainHeaderDynamicMeshCount(unsigned int dynamicMeshCount);
	void setMainHeaderStaticMeshCount(unsigned int nrOfMeshes);
	void setMainHeaderBoundBoxCount(unsigned int nrOfBoundingBoxMeshes);
	void setMainHeaderMaterialCount(unsigned int nrOfBoundingBoxMeshes);


	//MeshHeader
	void setMeshHeaderNameOfMesh(std::vector<char> nameOfMeshVector);
	void setMeshHeaderVertexCountOfMesh(unsigned int vertexCount);
	void setMeshHeaderCollisionOfMesh(bool collision);
	void setMeshHeaderStaticMesh(bool staticMesh);

	//BoundingBoxHeader
	void setBBHeaderNameOfBoundingBoxMesh(char nameOfBoundingBox[]);
	void setBBHeaderVertexCountOfBoundingBoxMesh(unsigned int vertexCount);
	void setBBHeaderCollisionOfBoundingBoxMesh(bool collision);
	void setBBHeaderStaticBoundingBoxMesh(bool staticBoundingBoxMesh);

	//Vertex
	void setMVHeaderPositionVertexOfMesh(float positionVertex[]);
	void setMVHeaderUVOfMesh(float UV[]);
	void setMVHeaderNormalOfMesh(float normal[]);

	//BoundingBoxVertex
	void setBBVHeaderPositionOfBoundingBox(float position[]);
	//--------------------------------------------------------------------




private:
	MainHeader m_mainHeader;
	MeshHeader m_meshHeader;
	BoundingBoxHeader m_boundingBoxMeshHeader;
	Vertex m_vertexHeader;
	BoundingBoxVertex m_boundingBoxVertexHeader;

};