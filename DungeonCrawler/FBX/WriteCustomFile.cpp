#include "WriteCustomFile.h"

WriteCustomFile::WriteCustomFile()
{
	m_mainHeader = MainHeader{ 1 }; //Creates one mainheader
}

WriteCustomFile::~WriteCustomFile()
{
	
}

void WriteCustomFile::CreateCustomFile()
{
	//Version Header
	unsigned int version = 0; //on top of file, once

	//Creates a scope of meshes based on the amount the ones counted in the mainheader staticmeshcount
	MeshHeader h { m_mainHeader.staticMeshCount };
	//Creates a vertex pointer to a new vertex array
	Vertex *vArray = new Vertex[h.vertexCount];

	/*
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
	*/
}

void WriteCustomFile::WriteMainHeader(int nrOfStaticMeshes, int nrOfBoundingBoxes) //works
{
	//CLEAN
	std::ofstream outfile("staticMeshBin.bin", std::ofstream::binary); //change
	outfile.close();


	/*
	m_mainHeader.version = '1';
	m_mainHeader.dynamicMeshCount = 0;
	m_mainHeader.staticMeshCount = nrOfStaticMeshes;
	m_mainHeader.boundingBoxCount = nrOfBoundingBoxes;

	//Write to file, make sure to write binary
	std::ofstream outfile("mainHeaderBin.bin", std::ofstream::binary);

	//add mainheader
	outfile.write((const char*)&m_mainHeader, sizeof(MainHeader));

	outfile.close();
	*/
}

void WriteCustomFile::WriteStaticMesh(StaticMesh currentMesh) //testing, I think it works
{
	MeshHeader lmeshHeader{ 1 };

	//Add data into the mesh header struct
	for (int i = 0; i < 100; i++)
	{
		lmeshHeader.nameOfMesh[i] = currentMesh.getNameCharacter(i);
	}

	lmeshHeader.vertexCount = currentMesh.getVertexCount();

	for (int i = 0; i < 100; i++)
	{
		lmeshHeader.vertexIndexArray[i] = currentMesh.getControlPointIndex(i);
	}

	for (int i = 0; i < 100; i++)
	{
		lmeshHeader.UVIndexArray[i] = currentMesh.getUVIndex(i);
	}

	lmeshHeader.collision = currentMesh.getCollision();
	lmeshHeader.staticMesh = currentMesh.getIsStatic();


	std::ofstream outfile;
	outfile.open("staticMeshBin.bin", std::ios::out | std::ios::app | std::ios::binary);

	outfile.write((const char*)&lmeshHeader, sizeof(MeshHeader)); //Writes meshHéader info
	//outfile.write((const char*)vArray, sizeof(Vertex)*lmeshHeader.vertexCount);	//writes all vertices

	//Creates a vertex pointer to a new vertex array
	//Vertex* vArray = new Vertex[lmeshHeader.vertexCount];

	/*
	//Something wrong here, but it works?
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vArray[i].position[j] = currentMesh.getControlPoint(i, j);
		}
		for (int j = 0; j < 2; j++)
		{
			vArray[i].UV[j] = currentMesh.getUVCoordinate(i, j);
		}
		for (int j = 0; j < 3; j++)
		{
			vArray[i].normal[j] = currentMesh.getNormal(i, j);
		}
	}
	for (int i = 0; i < lmeshHeader.vertexCount; i++)	 //WORKS, HMMM
	{
		printf("VERTEX TEST %i %.2f %.2f %.2f\n", i, 
			vArray[i].position[0], vArray[i].position[1], vArray[i].position[2]);
	}

	//this doesnt work
	//outfile.write((const char*)vArray, sizeof(Vertex));	//writes all vertices

	delete vArray; //need to delete all positions too?
	*/
	outfile.close();

	//currentMesh.CheckMesh();
}

void WriteCustomFile::WriteBoundingBoxMesh(BoundingBoxMesh currentMesh) //special case for boundingbox mesh with collision from current mesh to bounding box mesh header struct
{
	// IGNORE FOR NOW
	/*
	//debug prints out the data
	currentMesh.CheckMesh();

	m_mainHeader.boundingBoxCount += 1;
	BoundingBoxHeader boundingBoxHeader{ 1 };

	//Add data into the bounding box header struct
	boundingBoxHeader.vertexCount = currentMesh.getVertexCount();
	boundingBoxHeader.isStatic = currentMesh.getIsStatic();
	boundingBoxHeader.collision = currentMesh.getCollision();


	//Creates a boundingboxvertex pointer to a new boundingboxvertex array
	BoundingBoxVertex *bbvArray = new BoundingBoxVertex[boundingBoxHeader.vertexCount];
	*/
}

