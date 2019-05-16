#pragma once
#include "Headerstruct.h"
#include <vector>
#include <string>
#include <GL/glew.h> //maybe not needed?
#include <GLM/glm.hpp>

class FBXParserData 
{
private:
	//headers, maybe needed? maybe remove
	MainHeader m_mainHeader;
	MeshHeader m_meshHeader;
	Material m_materialHeader;
	BoundingBoxHeader m_boundingBoxHeader;
	Vertex m_vertexHeader;
	BoundingBoxVertex m_boundingBoxVertexHeader;

	//vertice data
	std::vector<glm::vec3> m_verticePos;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;

	//textures
	std::vector<std::string> m_albedoMapName;
	std::vector<std::string> m_normalMapName;

	//hitbox
	glm::vec4 m_maxMinValues;


public:
	FBXParserData(int vectorStartValue);
	~FBXParserData();
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
	void setMeshHeaderMaterialID(int materialID);
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
	
	//MaterialHeader
	void setMaterialHeaderNameOfAlbedo(std::vector<char> nameOfAlbedo);
	void setMaterialHeaderNameOfNormal(std::vector<char> nameOfNormal);

	//BoundingBoxVertex
	void setBBVHeaderPositionOfBoundingBox(float position[]);



	//ADD FUNCTIONS
	void addVertexPos(glm::vec3 vertexPos);
	void addUV(glm::vec2 uv);
	void addNormal(glm::vec3 normal);

	void addAlbedoMapName(std::string albedoMapName);
	void addNormalMapName(std::string normalMapName);

	//SET FUNCTIONS
	void setMaxMinValues(glm::vec4);

	//GET FUNCTIONS
	std::vector<glm::vec3> getVertexPos()const;
	std::vector<glm::vec2> getUVs()const;
	std::vector<glm::vec3> getNormals()const;

	std::vector<std::string> getAlbedoMapName()const;
	std::vector<std::string> getNormalMapName()const;

	glm::vec4 getMaxMinValues()const;
};