#pragma once

struct MainHeader
{
	char version; //What version of this program the file was made in,
	char padding1; //char also takes up 4 bytes, padd it
	char padding2;
	char padding3;
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
	int vertexIndexArray[100];	//SHOULD BE REMOVED LATER
	int UVIndexArray[100];		//SHOULD BE REMOVED LATER
	bool collision; //custom maya attribute
	bool staticMesh;	//custom maya attribute
	bool padding1; //Bool reserves 4 bytes, although it is only 1 byte, so might aswell padd them up
	bool padding2;
};

struct BoundingBoxHeader
{
	char nameOfHitbox[100];
	unsigned int vertexCount;
	int vertexIndexArray[36]; //can be exact cus box
	bool collision; //custom maya attribute
	bool staticMesh;	//custom maya attribute
	bool padding1; //Bool reserves 4 bytes, although it is only 1 byte, so might aswell padd them up
	bool padding2;
};

//Small info structs
struct Vertex
{
	float position[3];
	float UV[2];
	float normal[3];
};

struct BoundingBoxVertex
{
	float position[3];
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