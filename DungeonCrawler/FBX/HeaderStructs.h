#pragma once

struct MainHeader
{
	unsigned int dynamicMeshCount; //morph	/custom attribute for dynamic mesh?
	unsigned int staticMeshCount;  //regular
	unsigned int boundingBoxCount;
	unsigned int materialCount;
};

struct MeshHeader
{
	unsigned int vertexCount;
	unsigned int materialID; //our different morph characters will use the same material
	float UV[2]; //should this be here?
	char nameOfCustomAttribute[100]; //might not be needed
	bool collision; //custom maya attribute
};

struct BoundingBoxHeader
{
	unsigned int vertexCount;
};

//Small info structs
struct Vertex
{
	float position[3];
	float normal[3];
	float tangent[3];
	float biTangent[3];
};

struct BoundingBoxVertex
{
	float position[3];
	//indices? faces?
};

struct Material
{
	//texture buffer pointer
	//shader int pointer
};

struct Texture
{
	//texture buffer
};

struct ShaderOption
{
	//what shader it should use
};