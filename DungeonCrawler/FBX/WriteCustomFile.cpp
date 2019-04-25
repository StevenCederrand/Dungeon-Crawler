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
	m_mainHeader.version = '1';
	m_mainHeader.padding1 = ' ';
	m_mainHeader.padding2 = ' ';
	m_mainHeader.padding3 = ' ';
	m_mainHeader.dynamicMeshCount = 0;
	m_mainHeader.staticMeshCount = nrOfStaticMeshes;
	m_mainHeader.boundingBoxCount = nrOfBoundingBoxes;

	std::ofstream outfileBinary;
	outfileBinary.open("ourFileBinary.bin", std::ios::out | std::ios::trunc | std::ios::binary); //trunc to clean
	outfileBinary.write((const char*)&m_mainHeader, sizeof(MainHeader));
	outfileBinary.close();

	std::ofstream outfileReadable;
	outfileReadable.open("outFileReadable.txt", std::ios::out | std::ios::trunc);
	outfileReadable << "Version: " << m_mainHeader.version
		<< "\nNr of dynamic meshes: " << m_mainHeader.dynamicMeshCount
		<< "\nNr of static meshes: " << m_mainHeader.staticMeshCount 
		<< "\nNr of bounding boxes: " << m_mainHeader.boundingBoxCount << "\n\n";
}

void WriteCustomFile::WriteStaticMesh(StaticMesh currentMesh) //testing, I think it works
{
	MeshHeader lmeshHeader{ 1 };

	//Add data into the mesh header struct
	for (int i = 0; i < 100; i++)
	{
		lmeshHeader.nameOfMesh[i] = currentMesh.getNameCharacter(i);  //HAS $ IN SPOT 99?
	}

	lmeshHeader.vertexCount = currentMesh.getVertexCount();

	std::string vertexIndexArrString;
	for (int i = 0; i < 100; i++)
	{
		lmeshHeader.vertexIndexArray[i] = currentMesh.getControlPointIndex(i);
		if (i % 20 == 0)
			vertexIndexArrString += "\n";
		vertexIndexArrString += std::to_string(lmeshHeader.vertexIndexArray[i]);
		vertexIndexArrString += " ";
	}

	std::string UVIndexArrString;
	for (int i = 0; i < 100; i++)
	{
		lmeshHeader.UVIndexArray[i] = currentMesh.getUVIndex(i);
		if (i % 20 == 0)
			UVIndexArrString += "\n";
		UVIndexArrString += std::to_string(lmeshHeader.UVIndexArray[i]);
		UVIndexArrString += " ";
	}

	lmeshHeader.collision = currentMesh.getCollision();
	lmeshHeader.staticMesh = currentMesh.getIsStatic();
	lmeshHeader.padding1 = false;
	lmeshHeader.padding2 = false;

	std::ofstream outfileBinary;
	outfileBinary.open("ourFileBinary.bin", std::ios::out | std::ios::app | std::ios::binary); //writing, append, in binery
	outfileBinary.write((const char*)&lmeshHeader, sizeof(MeshHeader)); //Writes meshHeader info

	std::ofstream outfileReadable;
	outfileReadable.open("outFileReadable.txt", std::ios::out | std::ios::app);
	outfileReadable << "Name of mesh: " << lmeshHeader.nameOfMesh
		<< "\nVertex count: " << lmeshHeader.vertexCount
		<< "\n\nVertex index array: " << vertexIndexArrString.c_str()
		<< "\n\nUV index array: " << UVIndexArrString.c_str() << "\n\nCollision: " << lmeshHeader.collision
		<< "\nStatic mesh: " << lmeshHeader.staticMesh << "\n\n\n";

	//Creates a vertex pointer to a new vertex array
	Vertex* vArray = new Vertex[lmeshHeader.vertexCount];

	//I THINK IT WORKS, FLOAT NEEDS SPECIAL CONVERTER

	std::string vertexPosition;
	std::string vertexUV;
	std::string vertexNormal;
	for (int i = 0; i < lmeshHeader.vertexCount; i++)
	{
		vertexPosition += "Vertex: ";
		vertexPosition += std::to_string(i);

		vertexPosition += "\nPosition: ";
		for (int j = 0; j < 3; j++)
		{
			vArray[i].position[j] = currentMesh.getControlPoint(i, j);
			vertexPosition += std::to_string(vArray[i].position[j]);
			vertexPosition += " ";
		}
		vertexUV += "\nUV: ";
		for (int j = 0; j < 2; j++)
		{
			vArray[i].UV[j] = currentMesh.getUVCoordinate(i, j);
			vertexUV += std::to_string(vArray[i].UV[j]);
			vertexUV += " ";
		}
		vertexNormal += "\nnormal: ";
		for (int j = 0; j < 3; j++)
		{
			vArray[i].normal[j] = currentMesh.getNormal(i, j);
			vertexNormal += std::to_string(vArray[i].normal[j]);
			vertexNormal += " ";
		}
	}

	outfileBinary.write((const char*)vArray, sizeof(Vertex)*lmeshHeader.vertexCount);	//writes all vertices
	outfileBinary.close();

	
	SKRIV UT VERTEX INFON HÄR

	outfileReadable.close();
	

	delete vArray; //need to delete all positions too?
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



//STUFF I FIX: 
//BETTER CLEAN FUNCTION
//ADD READABLE FILE
//ADD SUPPORT TO WRITE BOUNDING BOX MESH TO FILE
//ADD MATERIALS



//TOBIAS
//START ON LOADER FOR THE REAL PROGRAM FOR THE FBX FILE