#pragma once

#include "DisplayCommon.h"
#include <vector>
#include "HeaderStructs.h"

class BoundingBoxMesh
{
private:
	char m_name[100];
	int m_vertexCount;
	bool m_collision;
	bool m_staticMesh;

	std::vector<std::vector<float>> m_controlPointsVECTOR;
	std::vector<int> m_controlPointIndexArrVECTOR;

	std::vector<BoundingBoxVertex> vertexArrVECTOR;

	void initiateArrays();
public:
	BoundingBoxMesh();
	~BoundingBoxMesh();
	void PrepareForNewMesh();
	void MakeAllTheVertices(int lNrOfVertices);

	void AddControlPoint(FbxVector4 controlPoint);
	void AddIndexPoint(int index);

	void setCollision(bool collision);
	void setStaticMesh(bool staticMesh);
	void setName(const char name[], int nameSize);

	char getNameCharacter(int space)const;
	bool getCollision()const;
	bool getIsStatic()const;
	int getVertexCount()const;
	std::vector<BoundingBoxVertex> getVertexArr()const;
};