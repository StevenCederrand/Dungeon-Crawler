#pragma once

#include "DisplayCommon.h"
#include <vector>
#include "HeaderStructs.h"

class BoundingBoxMesh
{
private:
	char m_name[100];

	std::vector<std::vector<float>> m_controlPointsVECTOR;
	std::vector<int> m_controlPointIndexArrVECTOR;

	bool m_collision;
	bool m_staticMesh;

	int m_nrOfPolygons;
	int m_nrOfVerticesPerPolygon; //3 if triangulated

	void initiateArrays();

	//Special things for sending full vertices
	int m_vertexCountVECTOR;
	std::vector<BoundingBoxVertex> vertexArrVECTOR;
public:
	BoundingBoxMesh();
	~BoundingBoxMesh();
	void PrepareForNewMesh();

	void MakeAllTheVertices(int lNrOfVertices);

	void AddControlPoint(FbxVector4 controlPoint);
	void AddIndexPoint(int index);

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
	std::vector<BoundingBoxVertex> getVertexArrVECTOR()const;
};