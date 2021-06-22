#include "pch.h"
#include "Mesh.h"
#include "..//FBX Reader/FBXLoader.h"

namespace radx
{
	Mesh::Mesh()
		: mpIndexBuffer(nullptr)
		, mpVertexBuffer(nullptr)
		, mIndexCount(0)
		, mVertexCount(0)
		, mpTexture(nullptr)
	{
	}

	Mesh::~Mesh()
	{
	}

	bool Mesh::Initialize(ID3D11Device* device)
	{
		HRESULT hr;

		mVertexCount = 24;
		mIndexCount = 36;

		CustomVertex* vertices = nullptr;
		std::vector<CustomVertex> v;
		
		FBXLoader fbx;
		fbx.LoadFBX((std::vector<TestVertex>*)&v, "Sphere.fbx");
		mVertexCount = v.size();

		vertices = new CustomVertex[mVertexCount];

		for (size_t i = 0; i < mVertexCount; i++)
		{
			vertices[i] = v[i];
		}

		//CustomVertex vertices[] =
		//{
		//	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		//	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		//	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		//	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

		//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		//	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		//	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

		//	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		//	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		//	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		//	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		//	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		//	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

		//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		//	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		//	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

		//	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		//	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		//	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		//	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		//};

		WORD indices[] =
		{
			3,1,0,
			2,1,3,

			6,4,5,
			7,4,6,

			11,9,8,
			10,9,11,

			14,12,13,
			15,12,14,

			19,17,16,
			18,17,19,

			22,20,21,
			23,20,22
		};


		////////////////////
		// Draw Triangle Textured
		////////////////////
		//mVertexCount = 3;
		//mIndexCount = 3;
		//CustomVertex vertices[3];
		//vertices[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		//vertices[0].Texture = XMFLOAT2(0.0f, 1.0f);
		//vertices[1].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);
		//vertices[1].Texture = XMFLOAT2(0.5f, 0.0f);
		//vertices[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		//vertices[2].Texture = XMFLOAT2(1.0f, 1.0f);

		//short indices[3];
		//indices[0] = 0;
		//indices[1] = 1;
		//indices[2] = 2;

		////////////////////
		// Draw Triangle
		////////////////////
		//mVertexCount = 3;
		//mIndexCount = 3;
		//CustomVertex vertices[3];
		//vertices[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		//vertices[0].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		//vertices[1].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);
		//vertices[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		//vertices[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		//vertices[2].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		//short indices[3];
		//indices[0] = 0;
		//indices[1] = 1;
		//indices[2] = 2;


		////////////////////
		// Draw Cube
		////////////////////
		//mVertexCount = 8;
		//mIndexCount = 36;
		//CustomVertex vertices[] =
		//{
		//	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		//	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		//	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		//	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		//	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		//	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		//};

		//WORD indices[] =
		//{
		//	3,1,0,
		//	2,1,3,

		//	0,5,4,
		//	1,5,0,

		//	3,4,7,
		//	0,4,3,

		//	1,6,5,
		//	2,6,1,

		//	2,7,6,
		//	3,7,2,

		//	6,4,5,
		//	7,4,6,
		//};


		D3D11_BUFFER_DESC bd;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(CustomVertex) * mVertexCount;
		bd.Usage = D3D11_USAGE_DEFAULT;
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		hr = device->CreateBuffer(&bd, &vertexData, &mpVertexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		delete[] vertices;

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * mIndexCount;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = indices;

		hr = device->CreateBuffer(&bd, &indexData, &mpIndexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		mpTexture = new Texture();
		mpTexture->Initialize(device, L"tex.dds");

		mRotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		mScale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

		return true;
	}

	void Mesh::Release()
	{
		if (mpTexture)
		{
			mpTexture->Release();
			mpTexture = nullptr;
		}

		if (mpVertexBuffer)
		{
			mpVertexBuffer->Release();
			mpVertexBuffer = nullptr;
		}
		if (mpIndexBuffer)
		{
			mpIndexBuffer->Release();
			mpIndexBuffer = nullptr;
		}
	}

	void Mesh::Load(ID3D11DeviceContext* deviceContext)
	{
		UINT stride = sizeof(CustomVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void Mesh::Move(float x, float y, float z)
	{
		mPosition.m128_f32[0] += x;
		mPosition.m128_f32[1] += y;
		mPosition.m128_f32[2] += z;
	}

	void Mesh::SetPosition(const XMVECTOR& vIn)
	{
		mPosition = vIn;
	}

	void Mesh::SetPosition(float x, float y, float z)
	{
		mPosition.m128_f32[0] = x;
		mPosition.m128_f32[1] = y;
		mPosition.m128_f32[2] = z;
	}

	void Mesh::SetScale(const XMVECTOR & vIn)
	{
		mScale = vIn;
	}

	void Mesh::SetScale(float x, float y, float z)
	{
		mScale.m128_f32[0] = x;
		mScale.m128_f32[1] = y;
		mScale.m128_f32[2] = z;
	}

	void Mesh::Rotate(float pitch, float yaw, float roll)
	{
		mRotation.m128_f32[0] += pitch;
		mRotation.m128_f32[1] += yaw;
		mRotation.m128_f32[2] += roll;
	}

	XMMATRIX Mesh::GetWorldMatrix() const
	{
		XMMATRIX ret = XMMatrixIdentity();
		ret *= XMMatrixScalingFromVector(mScale) * XMMatrixRotationRollPitchYawFromVector(mRotation) * XMMatrixTranslationFromVector(mPosition);
		return ret;
	}

	ID3D11ShaderResourceView* Mesh::GetTexture()
	{
		return mpTexture->GetTexture();
	}

}
