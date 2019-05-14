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
	unsigned int materialCount;
};

struct MeshHeader
{
	char nameOfMesh[100];
	unsigned int vertexCount;
	int materialID;  //which material it uses, needs to be able to be -1
	bool collision; //custom maya attribute
	bool staticMesh;	//custom maya attribute
	bool padding1; //Bool reserves 4 bytes, although it is only 1 byte, so might aswell padd them up
	bool padding2;
};

struct BoundingBoxHeader
{
	char nameOfHitbox[100];
	unsigned int vertexCount;
	bool collision; //custom maya attribute
	bool staticMesh;	//custom maya attribute
	bool padding1; //Bool reserves 4 bytes, although it is only 1 byte, so might aswell padd them up
	bool padding2;
};

struct SmallInfoHeader
{
	int intInfo;
	float floatInfo;
	char charInfo1;
	char charInfo2;
	char charInfo3;
	char charInfo4;
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

//Write all materials header after vertices, maybe liek this?
struct Material
{
	char nameOfAlbedo[100];
	char nameOfNormal[100];
	unsigned int materialID;
	unsigned int nrOfTextures; //Will be 2 right now, albedo and normalmap
	unsigned int whatShader;
	//texture buffer pointer
	//shader int pointer
};