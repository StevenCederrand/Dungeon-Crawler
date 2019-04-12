#include "SaveMeshInfo.h"

SaveMeshInfo::SaveMeshInfo()
{
	
}

SaveMeshInfo::~SaveMeshInfo()
{

}

void SaveMeshInfo::SaveControlPoints(FbxMesh* pMesh)
{
	int i = 0;
	int lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	printf("Control Points:\n");
	for (i = 0; i < lControlPointsCount; i++)
	{
		printf("Control Point %i ", i);
		Display3DVector("Coordinates: ", lControlPoints[i]);

		/*
		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "Normal Vetor: ");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
					Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
			}
		}
		*/
	}
	printf("\n\n");
}