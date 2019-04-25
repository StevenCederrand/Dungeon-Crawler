#pragma once

#include "DisplayCommon.h"

class StaticMesh
{
private:
	char m_name[100];
	int m_vertexCount;

	int m_currentControlPoint;			//Will not be needed when writing file
	int m_currentControlPointIndex;		//Will not be needed when writing file
	float m_controlPoints[3][100];
	int m_controlPointIndexArr[100];

	int m_currentUVCoordinate;			//Will not be needed when writing file
	int m_currentUVIndex;				//Will not be needed when writing file
	float m_UVCoordinates[2][100]; //They are 0 to 1, dont need to convert
	int m_UVCoordinateIndexArr[100];

	int m_currentNormalCoordinate;		//Will not be needed when writing file
	float m_normalCoordinateArr[3][100];
	//No index arr

	bool m_collision;
	bool m_staticMesh;

	int m_nrOfPolygons;					//Will not be needed when writing file
	int m_nrOfVerticesPerPolygon;		//Will not be needed when writing file,	3 if triangulated

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

	char getNameCharacter(int space)const;
	int getVertexCount()const;
	float getControlPoint(int i, int j)const;
	int getControlPointIndex(int index)const;
	float getUVCoordinate(int i, int j)const;
	int getUVIndex(int index)const;
	float getNormal(int i, int j)const;
	bool getCollision()const;
	bool getIsStatic()const;
	int getNrOfControlPoints()const;
};