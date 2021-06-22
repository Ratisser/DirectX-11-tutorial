#pragma once

#include <vector>
#include <fbxsdk.h>
#include <cassert>

#pragma comment(lib, "libfbxsdk.lib")

namespace radx
{
	struct TestVertex
	{
		float Position[3];
		float Normal[3];
		float Texture[2];
	};

	class FBXLoader
	{
	public:
		FBXLoader();
		~FBXLoader();

		bool LoadFBX(std::vector<TestVertex>* pOutVertexVector, const char* filePath);

	private:
		::FbxManager* mpFbxManager;

	};
}