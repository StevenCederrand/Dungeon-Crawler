#pragma once

#include "DisplayCommon.h"
#include <vector>
#include "HeaderStructs.h"

//MAKE EVERYTHING DYNAMIC HERE, MAKE NOTHING DYNAMIC IN WRITECUSTOMFILE

// NR1: ADD VECTOR TO ALL ARRAYS IN THIS FILE - DONE
// NR2: MAKE FUNCTIONS FOR THEM IN THIS FILE, AND MAKE SURE IT WORKS WHEN USED IN SAVEHIAERARCHY,
//		SO ALL FUNCTIONS CAN BE REPLACED WITH THOSE FOR THE VECTORS - DONE
//		
// NR2.5: MAKE A NEW FILE THAT WRITES FULL VERTICES - DONE
// NR3: REMOVE THE OLD ARRAYS AND USE ONLY VECTORS, NOW CAN SUPPORT UNLIMITED AMOUNT OF VERTICES
// NR4: REMOVE UNUSED VARIABLES
// NR5: WRITE THE VERTICES TO THE FILE

//WRITE NO INDEXES TO THE FILE
//WRITE ONLY VERTICES IN THE WAY THEY WILL BE USED

class StaticMesh
{
private:
	char m_name[100];
	std::vector<std::vector<float>> m_controlPointsVECTOR;
	std::vector<int> m_controlPointIndexArrVECTOR;

	std::vector<std::vector<float>> m_UVCoordinatesVECTOR;
	std::vector<int> m_UVCoordinateIndexArrVECTOR;

	std::vector<std::vector<float>> m_normalCoordinateArrVECTOR;

	bool m_collision;
	bool m_staticMesh;

	int m_nrOfPolygons;					//Will not be needed when writing file
	int m_nrOfVerticesPerPolygon;		//Will not be needed when writing file,	3 if triangulated

	void initiateArrays();

	//Special things for sending full vertices
	int m_vertexCountVECTOR;
	std::vector<Vertex> vertexArrVECTOR;

public:
	StaticMesh();
	~StaticMesh();
	void PrepareForNewMesh();

	void MakeAllTheVertices(int lNrOfVertices);

	void AddControlPoint(FbxVector4 controlPoint);
	void AddIndexPoint(int index);
	void AddUVCoordinate(FbxVector2 uVCoordinate);
	void AddUVIndex(int index);
	void AddNormalCoordinate(FbxVector4 normalCoordinate);

	void setNrOfPolygons(int nrOfPolygons);
	void setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon);
	void setCollision(bool collision);
	void setStaticMesh(bool staticMesh);
	void setName(const char name[], int nameSize);

	char getNameCharacter(int space)const;
	bool getCollision()const;
	bool getIsStatic()const;

	//VECTOR PART
	int getVertexCountVECTOR()const;
	std::vector<Vertex> getVertexArrVECTOR()const;
};

/*
MeshHeader myHeader;
vector<Vertex> Vertices;

-go through FBX file, through control points and polygons and fill in the vertices array.
- pushing each vertex into Vertices array.
---------------------- -
myHeader.vertexCount = Vertices.size();
myHeader....
write myHeader to disk.
file.write(Vertices.data(), sizeof(Vertex) * vertexCount);
*/