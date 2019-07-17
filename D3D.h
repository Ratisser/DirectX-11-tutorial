#pragma once

#include "Mesh.h"
#include "Camera.h"

namespace radx
{
	class D3D
	{
	private:
		struct CustomVertex
		{
			XMFLOAT3 Position;
			XMFLOAT4 Color;
		};

		struct ConstantBuffer
		{
			XMMATRIX World;
			XMMATRIX View;
			XMMATRIX Projection;
		};

	public:
		D3D();
		D3D(const D3D& other) = delete;
		D3D operator=(const D3D& rhs) = delete;
		~D3D();

		bool Initialize(HWND hWnd, bool bVsync, UINT clientWidth, UINT clientHeight);
		bool LoadAsset();
		void ReleaseAsset();
		void Release();

		void Frame();

		const char* GetVideoCardDescription();

	private:
		void UpdateScene();
		void BeginScene(float red, float green, float blue, float alpha);
		void EndScene();
	private:
		enum { VIDEO_CARD_DESC_SIZE = 128 };

		HWND						mhWnd;
		UINT						mClientWidth;
		UINT						mClientHeight;
		ID3D11Device*				mDevice;
		ID3D11DeviceContext*		mDeviceContext;
		IDXGISwapChain*				mSwapChain;
		ID3D11RenderTargetView*		mRenderTargetView;

		ID3D11VertexShader*			mVertexShader;
		ID3D11InputLayout*			mVertexLayout;
		ID3D11Buffer*				mConstantBuffer;
		ID3D11PixelShader*			mPixelShader;

		Mesh						mMesh;
		Camera						mCamera;
		XMMATRIX					mWorld;
		XMMATRIX					mView;
		XMMATRIX					mProjection;

		
		bool mbVsync;
		int mVideoCardMemory;
		char mVideoCardDescription[VIDEO_CARD_DESC_SIZE];
	};
}

