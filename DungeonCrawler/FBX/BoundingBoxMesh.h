#pragma once

#include "DisplayCommon.h"
#include <vector>
#include "HeaderStructs.h"

class BoundingBoxMesh
{
private:
	char m_name[100];
	int m_vertexCount;

	int m_currentControlPoint;				//Will not be needed when writing file
	int m_currentControlPointIndex;			//Will not be needed when writing file
	float m_controlPoints[3][8];
	std::vector<std::vector<float>> m_controlPointsVECTOR;
	int m_controlPointIndexArr[36];
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

	void MakeAllTheVertices();

	void AddControlPoint(FbxVector4 controlPoint);
	void AddIndexPoint(int index);

	void IncreaseVertexCount();
	void CheckMesh();

	void setNrOfPolygons(int nrOfPolygons);
	void setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon);
	void setCollision(bool collision);
	void setStaticMesh(bool staticMesh);
	void setName(const char name[], int nameSize);

	char getNameCharacter(int space)const;
	bool getCollision()const;
	bool getIsStatic()const;
	int getVertexCount()const;
	float getControlPoint(int i, int j)const;
	int getControlPointIndex(int index)const;

	//VECTOR PART
	int getVertexCountVECTOR()const;
	std::vector<BoundingBoxVertex> getVertexArrVECTOR()const;
};