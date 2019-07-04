#pragma once
// TODO : Object에 있어야 할 요소가 있음. 나중에 리팩토링
namespace radx
{
	class Mesh
	{
	private:
		struct CustomVertex
		{
			XMFLOAT3		Position;
			XMFLOAT4		Color;
		};

	public:
		Mesh();
		~Mesh();

		bool				Initialize(ID3D11Device* device);
		void				Release();
		void				Load(ID3D11DeviceContext* deviceContext);
		

		void				Move(float x, float y, float z);
		void				MoveForward(float distance);
		void				SetPosition(const XMVECTOR& vIn);
		void				Rotate(float pitch, float yaw, float roll);
		int					GetVertexCount() const { return mVertexCount; }
		int					GetIndexCount()	const { return mIndexCount; }
		XMMATRIX			GetWorldMatrix() const;

	private:
		ID3D11Buffer*		mVertexBuffer;
		ID3D11Buffer*		mIndexBuffer;
		int					mVertexCount;
		int					mIndexCount;
		XMVECTOR			mPosition;
		XMVECTOR			mRotation;
		XMVECTOR			mScale;

	};
}
