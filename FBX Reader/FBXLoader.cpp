#include "FBXLoader.h"
#include "framework.h"

radx::FBXLoader::FBXLoader()
	: mpFbxManager(nullptr)
{
}

radx::FBXLoader::~FBXLoader()
{
}

bool radx::FBXLoader::LoadFBX(std::vector<TestVertex>* pOutVertexVector, const char* filePath)
{
	if (mpFbxManager == nullptr)
	{
		mpFbxManager = FbxManager::Create();

		FbxIOSettings* pIOSettings = FbxIOSettings::Create(mpFbxManager, IOSROOT);
		mpFbxManager->SetIOSettings(pIOSettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(mpFbxManager, "");
	FbxScene* pFbxScene = FbxScene::Create(mpFbxManager, "");

	bool bSuccess = pImporter->Initialize(filePath, -1, mpFbxManager->GetIOSettings());
	if (!bSuccess)
	{
		return false;
	}

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess)
	{
		return false;
	}

	pImporter->Destroy();

	FbxNode* pRootNode = pFbxScene->GetRootNode();

	if (pRootNode)
	{
		for (int i = 0; i < pRootNode->GetChildCount(); i++)
		{
			FbxNode* pChildNode = pRootNode->GetChild(i);

			if (pChildNode->GetNodeAttribute() == NULL)
			{
				continue;
			}

			FbxNodeAttribute::EType attributeType = pChildNode->GetNodeAttribute()->GetAttributeType();

			if (attributeType != FbxNodeAttribute::eMesh)
			{
				continue;
			}

			FbxMesh* pMesh = (FbxMesh*)pChildNode->GetNodeAttribute();

			FbxVector4* pVertices = pMesh->GetControlPoints();
			int polygonCount = pMesh->GetPolygonCount();

			for (int j = 0; j < polygonCount; j++)
			{
				int nVertices = pMesh->GetPolygonSize(j); // 삼각형이 아니면 안 되니까
				//if (nVertices != 3)
				//{
				//	continue;
				//}

				


				for (int k = 0; k < nVertices; k++)
				{
					TestVertex vertex;
					FbxVector4 normal;
					int controlPointIndex = pMesh->GetPolygonVertex(j, k);
					pMesh->GetPolygonVertexNormal(j, k, normal);
					

					vertex.Position[0] = (float)pVertices[controlPointIndex].mData[0];
					vertex.Position[1] = (float)pVertices[controlPointIndex].mData[1];
					vertex.Position[2] = (float)pVertices[controlPointIndex].mData[2];
					vertex.Normal[0] = (float)normal[0];
					vertex.Normal[1] = (float)normal[1];
					vertex.Normal[2] = (float)normal[2];

					pOutVertexVector->push_back(vertex);
				}
			}

		}
	}

	return true;
}
