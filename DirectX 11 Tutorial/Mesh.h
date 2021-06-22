#pragma once
// TODO : Object에 있어야 할 요소가 있음. 나중에 리팩토링

#include "Texture.h"

namespace radx
{
	class Mesh
	{
	private:
		//struct CustomVertex
		//{
		//	XMFLOAT3				Position;
		//	XMFLOAT4				Color;
		//};

		struct CustomVertex
		{
			XMFLOAT3				Position;
			XMFLOAT3				Normal;
			XMFLOAT2				Texture;
		};

	public:
		Mesh();
		~Mesh();

		bool						Initialize(ID3D11Device* device);
		void						Release();
		void						Load(ID3D11DeviceContext* deviceContext);
		

		void						Move(float x, float y, float z);
		void						MoveForward(float distance);
		void						MoveBackward(float distance);
		void						StrafeLeft(float distance);
		void						StrafeRight(float distance);
		void						SetPosition(const XMVECTOR& vIn);
		void						SetPosition(float x, float y, float z);
		void						SetScale(const XMVECTOR& vIn);
		void						SetScale(float x, float y, float z);
		void						Rotate(float pitch, float yaw, float roll);
		int							GetVertexCount() const { return mVertexCount; }
		int							GetIndexCount()	const { return mIndexCount; }
		XMMATRIX					GetWorldMatrix() const;
		ID3D11ShaderResourceView*	GetTexture();

	private:
		ID3D11Buffer*				mpVertexBuffer;
		ID3D11Buffer*				mpIndexBuffer;
		int							mVertexCount;
		int							mIndexCount;
		XMVECTOR					mPosition;
		XMVECTOR					mRotation;
		XMVECTOR					mScale;
		
		Texture*					mpTexture;
	};
}
