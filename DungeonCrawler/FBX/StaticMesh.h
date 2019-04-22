#pragma once

#include "DisplayCommon.h"

class StaticMesh
{
private:
	char m_name[100];
	int m_vertexCount;

	int m_currentControlPoint;
	int m_currentControlPointIndex;
	float m_controlPoints[3][100];
	int m_controlPointIndexArr[100];

	int m_currentUVCoordinate;
	int m_currentUVIndex;
	float m_UVCoordinates[2][100]; //They are 0 to 1, dont need to convert
	int m_UVCoordinateIndexArr[100];

	int m_currentNormalCoordinate;
	float m_normalCoordinateArr[3][100];	//change to a vector 3

	bool m_collision;
	bool m_isStatic;

	int m_nrOfPolygons;
	int m_nrOfVerticesPerPolygon; //3 if triangulated

	void initiateArrays();
public:
	StaticMesh();
	~StaticMesh();
	void PrepareForNewMesh();
	void AddControlPoint(FbxVector4 controlPoint);
	void AddIndexPoint(int index);
	void AddUVCoordinate(FbxVector2 uVCoordinate);
	void AddUVIndex(int index);
	void AddNormalCoordinate(FbxVector4 normalCoordinate);
	void increaseVertexCount();
	void CheckMesh();

	void setNrOfPolygons(int nrOfPolygons);
	void setNrOfVerticesPerPolygon(int nrOfVerticesPerPolygon);
	void setCollision(bool collision);
	void setStaticMesh(bool staticMesh);
	void setName(const char name[], int nameSize);

	bool getCollision()const;
	bool getIsStatic()const;
};

//different meshdata for each unic data, like hitbox only 4 pos and its speciall data is collision= true