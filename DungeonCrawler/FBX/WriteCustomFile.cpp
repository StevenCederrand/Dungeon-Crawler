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
}

void WriteCustomFile::GatherMeshData()
{
	//Continue with this
}