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

	//Creates a scope of 100 meshheaders
	MeshHeader h { 100 };
	BoundingBoxHeader bh { 100 };

	//Creates a vertex pointer to a new vertex array
	Vertex *vArray = new Vertex[h.vertexCount];
	BoundingBoxVertex *bbvArray = new BoundingBoxVertex[bh.vertexCount];

	//Write to file
	std::ofstream outfile("testCustomBin.bin", std::ofstream::binary); //make a new file, make sure to write binary
	
	//add info to header	
	outfile.write((const char*)&h, sizeof(MeshHeader));
	//add info to header
	outfile.write((const char*)vArray, sizeof(Vertex)*h.vertexCount);

	//add info to header
	outfile.write((const char*)&bh, sizeof(BoundingBoxHeader));
	//add info to header
	outfile.write((const char*)bbvArray, sizeof(BoundingBoxVertex)*h.vertexCount);

	outfile.close();

	//Read from file
	std::ifstream infile("testCustomBin.bin", std::ifstream::binary); //make sure to read binary

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
	std::cout << "Streams are equal, method 2: " << equal << "\n" << std::endl;
}
