#include "pch.h"
#include "Mesh.h"

namespace radx
{
	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
	}

	bool Mesh::Initialize(ID3D11Device* device)
	{
		HRESULT hr;
		CustomVertex vertices[] =
		{
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		};

		mVertexCount = 8;

		D3D11_BUFFER_DESC bd;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(CustomVertex) * mVertexCount;
		bd.Usage = D3D11_USAGE_DEFAULT;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = vertices;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		hr = device->CreateBuffer(&bd, &initData, &mVertexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		WORD indices[] =
		{
			3,1,0,
			2,1,3,

			0,5,4,
			1,5,0,

			3,4,7,
			0,4,3,

			1,6,5,
			2,6,1,

			2,7,6,
			3,7,2,

			6,4,5,
			7,4,6,
		};

		mIndexCount = 36;

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * mIndexCount;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		initData.pSysMem = indices;

		hr = device->CreateBuffer(&bd, &initData, &mIndexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		mRotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		mScale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

		return true;
	}

	void Mesh::Release()
	{
		if (mVertexBuffer)
		{
			mVertexBuffer->Release();
			mVertexBuffer = nullptr;
		}
		if (mIndexBuffer)
		{
			mIndexBuffer->Release();
			mIndexBuffer = nullptr;
		}
	}

	void Mesh::Load(ID3D11DeviceContext* deviceContext)
	{
		UINT stride = sizeof(CustomVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
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

}
