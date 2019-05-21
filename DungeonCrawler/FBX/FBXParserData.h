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
	//Maybe vector of vectors?
	MainHeader m_mainHeader;
	MeshHeader m_meshHeader;	//should be vector
	Material m_materialHeader;	//should be vector
	std::vector<BoundingBoxHeader> m_boundingBoxHeaders;
	Vertex m_vertexHeader;
	BoundingBoxVertex m_boundingBoxVertexHeader;

	//vertice data
	std::vector<glm::vec3> m_verticePos;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;

	//textures, DONT USE; USE THE HEADER
	std::vector<std::string> m_albedoMapName;
	std::vector<std::string> m_normalMapName;

	//Mesh
	glm::vec4 m_minMaxValuesMesh;

	//hitbox
	std::vector<glm::vec3> m_minMaxValuesHitbox;


public:
	FBXParserData(int vectorStartValue);
	~FBXParserData();
	void setVertexHeader(Vertex vertexHeader);

	//-----------------------------------------------------------------
	//Struct getters functions in order
	MainHeader getMainHeader()const;
	MeshHeader getMeshHeader()const;
	Material getMaterialHeader()const;
	std::vector<BoundingBoxHeader> getBoundingBoxHeaders()const;
	Vertex getVertexHeader()const;
	BoundingBoxVertex getBoundingBoxVertexHeader()const;


	//ADD FUNCTIONS
	void addVertexPos(glm::vec3 vertexPos);
	void addUV(glm::vec2 uv);
	void addNormal(glm::vec3 normal);

	void addAlbedoMapName(std::string albedoMapName);
	void addNormalMapName(std::string normalMapName);

	void addMinMaxValuesHitbox(glm::vec3 maxMinValuesHitbox);

	void addBoundingBoxHeader(BoundingBoxHeader boundingBoxHeader);

	//SET FUNCTIONS
	void setMinMaxValuesMesh(glm::vec4);
	//headers
	void setMainHeader(MainHeader mainHeader);
	void setMeshHeader(MeshHeader meshHeader);
	void setMaterialHeader(Material material);

	//GET FUNCTIONS
	std::vector<glm::vec3> getVertexPos()const;
	std::vector<glm::vec2> getUVs()const;
	std::vector<glm::vec3> getNormals()const;

	std::vector<std::string> getAlbedoMapName()const;
	std::vector<std::string> getNormalMapName()const;

	glm::vec4 getMinMaxValuesMesh()const;					//ACTUALLY IS MINMAX
	std::vector<glm::vec3> getMinMaxValuesHitbox()const;	//ACTUALLY IS MINMAX
};