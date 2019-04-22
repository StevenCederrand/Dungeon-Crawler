#include "WriteCustomFile.h"

WriteCustomFile::WriteCustomFile()
{

}

WriteCustomFile::~WriteCustomFile()
{
	
}

void WriteCustomFile::CreateCustomFile()
{
	//IGNORE FOR NOW, START WITH FBX FILE

	//Version Header
	unsigned int version = 0; //on top of file, once

	//Creates one mainheader
	MainHeader mh{ 1 };

	//Creates a scope of meshes based on the amount the ones counted in the mainheader scene
	MeshHeader h { mh.staticMeshCount };
	//Creates a vertex pointer to a new vertex array
	Vertex *vArray = new Vertex[h.vertexCount];

	//Creates a scope of boundingboxheader classes based on the amount of boundingbox counted
	BoundingBoxHeader bbh { mh.boundingBoxCount };
	//Creates a boundingboxvertex pointer to a new boundingboxvertex array
	BoundingBoxVertex *bbvArray = new BoundingBoxVertex[bbh.vertexCount];

	//Write to file
	std::ofstream outfile("testCustomBin.bin", std::ofstream::binary); //make a new file, make sure to write binary
	
	//add info to header	
	outfile.write((const char*)&mh, sizeof(MainHeader));

	//add info to header	
	outfile.write((const char*)&h, sizeof(MeshHeader));
	//add info to header
	outfile.write((const char*)vArray, sizeof(Vertex)*h.vertexCount);

	//add info to header
	outfile.write((const char*)&bbh, sizeof(BoundingBoxHeader));
	//add info to header
	outfile.write((const char*)bbvArray, sizeof(BoundingBoxVertex)*bbh.vertexCount);

	outfile.close();

	//Read from file
	std::ifstream infile("testCustomBin.bin", std::ifstream::binary); //make sure to read binary

	//read the first 4 bytes and put into h2
	MeshHeader h2;
	infile.read((char*)&h2, sizeof(MeshHeader));

	//Default construction of Vertex struct
	Vertex *vertices = new Vertex[h2.vertexCount];

	//read the first 4 bytes and put into bbh2
	BoundingBoxHeader bbh2;
	infile.read((char*)&bbh2, sizeof(MeshHeader));

	//Default construction of Vertex struct
	BoundingBoxVertex *bbvertices = new BoundingBoxVertex[bbh2.vertexCount];


	//Read all the vertices on the file
	infile.read((char*)vertices, h2.vertexCount * sizeof(Vertex));
	infile.read((char*)bbvertices, bbh2.vertexCount * sizeof(Vertex));
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
	std::cout << "Streams are equal, method 2: " << equal << "\n" << std::endl;
}

void WriteCustomFile::WriteStaticMesh(StaticMesh currentMesh) //speciall case for static mesh no collision
{
	currentMesh.CheckMesh();
}

void WriteCustomFile::WriteBoundingBoxMesh(BoundingBoxMesh currentMesh)
{
	currentMesh.CheckMesh();
}

