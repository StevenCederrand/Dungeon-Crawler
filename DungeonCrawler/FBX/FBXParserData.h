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
	std::vector<MeshHeader> m_meshHeaders;	//should be vector
	std::vector<Material> m_materialHeaders;	//should be vector
	std::vector<BoundingBoxHeader> m_boundingBoxHeaders;
	Vertex m_vertexHeader;
	BoundingBoxVertex m_boundingBoxVertexHeader;

	//vertice mesh data
	//maybe make vector of vectors? each one for a new mesh
	std::vector<std::vector<glm::vec3>> m_verticePosVector; //Add m_verticePos to this each time one is done.

	std::vector<glm::vec3> m_verticePos;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	//vertice hitbox data
	std::vector<glm::vec3> m_verticePosHitbox;	//maybe make vector of vectors?

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
	std::vector<BoundingBoxHeader> getBoundingBoxHeaders()const;
	Vertex getVertexHeader()const;
	BoundingBoxVertex getBoundingBoxVertexHeader()const;

	void moveVerticePosToVector();

	//ADD FUNCTIONS
	void addVertexPos(glm::vec3 vertexPos);
	void addUV(glm::vec2 uv);
	void addNormal(glm::vec3 normal);

	void addVertexPosHitbox(glm::vec3 vertexPosHibox);

	void addAlbedoMapName(std::string albedoMapName);
	void addNormalMapName(std::string normalMapName);

	void addMinMaxValuesHitbox(glm::vec3 maxMinValuesHitbox);

	//headers
	void addBoundingBoxHeader(BoundingBoxHeader boundingBoxHeader);
	void addMeshHeader(MeshHeader meshHeader);
	void addMaterialHeader(Material materialHeader);

	//SET FUNCTIONS
	void setMinMaxValuesMesh(glm::vec4);
	//headers
	void setMainHeader(MainHeader mainHeader);

	//GET FUNCTIONS
	std::vector<glm::vec3> getVertexPos()const;
	std::vector<glm::vec2> getUVs()const;
	std::vector<glm::vec3> getNormals()const;
	std::vector<std::vector<glm::vec3>> getVerticePosVector()const;

	std::vector<glm::vec3> getVertexPosHitbox()const;

	std::vector<std::string> getAlbedoMapName()const;
	std::vector<std::string> getNormalMapName()const;

	glm::vec4 getMinMaxValuesMesh()const;					//ACTUALLY IS MINMAX
	std::vector<glm::vec3> getMinMaxValuesHitbox()const;	//ACTUALLY IS MINMAX
	//headers
	std::vector<MeshHeader> getMeshHeaders()const;
	std::vector<Material> getMaterialHeaders()const;
};