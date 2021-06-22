#pragma once

#include "Mesh.h"
#include "Camera.h"

namespace radx
{
 	__declspec(align(16)) class D3D
	{
	private:
		struct ConstantBuffer
		{
			XMMATRIX World;
			XMMATRIX View;
			XMMATRIX Projection;
		};
		
		struct LightBuffer
		{
			XMVECTOR LightPos;
		};

		struct CameraBuffer
		{
			XMVECTOR CamPos;
		};

	public:
		D3D();
		D3D(const D3D& other) = delete;
		D3D operator=(const D3D& rhs) = delete;
		~D3D();

		bool Initialize(HWND hWnd, bool bVsync, UINT clientWidth, UINT clientHeight);
		bool SetFrameRate();
		bool LoadAsset();
		void ReleaseAsset();
		void Release();

		void Frame();

		const char* GetVideoCardDescription();

	protected:
		void updateScene();
		void beginScene(float red, float green, float blue, float alpha);
		void EndScene();

	private:
		enum { VIDEO_CARD_DESC_SIZE = 128 };

		ID3D11Device*				mpDevice;
		ID3D11DeviceContext*		mpDeviceContext;
		IDXGISwapChain*				mpSwapChain;
		ID3D11RenderTargetView*		mpRenderTargetView;
		ID3D11Texture2D*			mpDepthStencil;
		ID3D11DepthStencilView*		mpDepthStencilView;

		// TODO : ºŒ¿Ã¥ı ∫–∏Æ
		ID3D11VertexShader*			mpVertexShader;
		ID3D11InputLayout*			mpVertexLayout;
		ID3D11Buffer*				mpConstantBuffer;
		ID3D11Buffer*				mpLightBuffer;
		ID3D11Buffer*				mpCameraBuffer;
		ID3D11PixelShader*			mpPixelShader;
		ID3D11SamplerState*			mpSamplerState;


		Mesh						mMesh;
		Mesh						mMesh2;
		Mesh						mMesh3;
		Camera						mCamera;
		XMMATRIX					mWorld;
		XMMATRIX					mView;
		XMMATRIX					mProjection;

		HWND						mhWnd;
		UINT						mClientWidth;
		UINT						mClientHeight;
		bool						mbVsync;
		UINT						mFrameRate;
		int							mVideoCardMemory;
		char						mVideoCardDescription[VIDEO_CARD_DESC_SIZE];
	};
}

