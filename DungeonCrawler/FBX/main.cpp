#include <fbxsdk.h>

#include "DisplayCommon.h"
#include "fbxObjectManager.h"
#include "DisplayHierarchy.h"

#define DELTA 0.0001
#define EQUAL(A,B) (abs((A)-(B)) < DELTA) ? true:false

//Declerations of all functions
void CreateCustomFile();

/*
//Header Structs
struct MainHeader
{
	unsigned int meshCount;
	unsigned int materialCount;
	unsigned int characterCount;
};

struct MeshHeader 
{
	unsigned int vertexCount;
	unsigned int materialID; //our different morph bois will use the same material
	unsigned int UV[2]; //should this be here? int?
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

class Material
{
	private:
		//texture buffer pointer
		//shader int pointer
};

class Texture
{
	private:
		//texture buffer
};

class ShaderOption
{
	private:
		//what shader it should use
};
*/
void CreateCustomFile()
{
	/*
	//IGNORE FOR NOW, START WITH FBX FILE

	//HeadersAndStuff
	MeshHeader h { 100 };
	Vertex *vArray = new Vertex[h.vertexCount];
	//Write to file
	std::ofstream outfile("testBin.bin", std::ofstream::binary); //make a new file, make sure to write binary
	outfile.write((const char*)&h, sizeof(MeshHeader));
	//add info to header
	outfile.write((const char*)vArray, sizeof(Vertex)*h.vertexCount);
	outfile.close();

	//Read from file
	std::ifstream infile("testBin.bin", std::ifstream::binary); //make sure to read binary

	//read the first 4 bytes and put into h2
	MeshHeader h2;
	infile.read((char*)&h2, sizeof(MeshHeader));


	h2.vertexCount; //- a lot
	//Default construction of Vertex struct
	Vertex *vertices = new Vertex[h2.vertexCount];
	//Read all the vertices on the file
	infile.read((char*)vertices, h2.vertexCount * sizeof(Vertex));
	infile.close();

	//comparison:
	bool equal = true;
	for (int i = 0; i < h.vertexCount; i++)
	{
		if (!EQUAL(vArray[i].position[0], vertices[i].position[0]) ||
			!EQUAL(vArray[i].position[1], vertices[i].position[1]) ||
			!EQUAL(vArray[i].position[2], vertices[i].position[2]))
		{
			equal = false;
			break;
		}
	}
	std::cout << "Streams are equal, method 2: " << equal << std::endl;
	*/
}


int main(int argc, char** argv) 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//createCustomFile();

	//Make objects like SDK manager, scene, importer
	//Imports FBX file and cleans importer
	fbxObjectManager lsdkObject = fbxObjectManager();

	//Display our FBX file hierarchy
	DisplayHierarchy displayHierarchyObject;
	displayHierarchyObject.DisplayEntireHierarchy(lsdkObject.GetScene());
	
	return 0;
}