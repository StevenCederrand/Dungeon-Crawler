#pragma once

struct MainHeader
{
	char version; //What version of this program the file was made in
	unsigned int dynamicMeshCount; //morph	//custom attribute
	unsigned int staticMeshCount;  //regular
	unsigned int boundingBoxCount;
	//unsigned int materialCount;
};

struct MeshHeader
{
	char nameOfMesh[100];
	unsigned int vertexCount;
	//unsigned int materialID; //our different morph characters will use the same material
	int vertexIndexArray[100];
	int UVIndexArray[100];
	bool collision; //custom maya attribute
	bool isStatic;	//custom maya attribute
};

struct BoundingBoxHeader
{
	unsigned int vertexCount;
	bool collision; //custom maya attribute
	bool isStatic;	//custom maya attribute
};

//Small info structs
struct Vertex
{
	float position[3];
	float normal[3];
	float UV[2];
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