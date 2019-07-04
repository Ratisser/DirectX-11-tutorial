#include "pch.h"
#include "D3D.h"

namespace radx
{
	D3D::D3D()
	{
	}

	D3D::~D3D()
	{
	}

	bool D3D::Initialize(HWND hWnd, bool bVsync, UINT clientWidth, UINT clientHeight)
	{
		HRESULT hr;
		mbVsync = bVsync;

		// Direct3D�� �ʱ�ȭ�ϱ� �� �׷��� ī��� ����Ϳ��� refresh rate�� ���� �Ѵٰ� ��.
		// �׷��� �ʰ� refresh rate�� ���Ƿ� �����ϸ� DirectX�� ���� flip ��� blt�Ͽ� ������ ���ϵǰ� ����� ��忡 ������ �߻���Ŵ

		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		IDXGIFactory* factory;

		DXGI_MODE_DESC* displayModeList;
		UINT nModes;
		DXGI_ADAPTER_DESC adapterDesc;

		UINT numerator = 60;
		UINT denominator = 1;

		// DirextX Graphics Interface Factory �� �����մϴ�.
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(hr))
		{
			return false;
		}

		// ���丮�� �̿��ؼ� �� �׷���ī���� ����͸� �����մϴ�.
		hr = factory->EnumAdapters(0, &adapter);
		if (FAILED(hr))
		{
			return false;
		}

		// �⺻ ������� ����� �����մϴ�.
		hr = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(hr))
		{
			return false;
		}

		// ������� ��¿� ���� DXGI_FORMAT_R8G8B8A8_UNORM ǥ�� ���Ŀ� �´� ��� ���� �����ɴϴ�.
		// (UTF8���ڿ��� UTF16���ڿ��� �ٲܶ� ó�� �ϴ� ����� ���� ���ϹǷ�
		// ������ ���ڿ� NULL�� ����)
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nModes, nullptr);
		if (FAILED(hr))
		{
			return false;
		}

		// ����Ϳ� �׷��� ī�� ���տ� ������ ��� ǥ�� ��带 ���� ����� ����ϴ�.
		displayModeList = new DXGI_MODE_DESC[nModes];
		if (!displayModeList)
		{
			return false;
		}

		// ���÷��� ��� ��� ������ ä��ϴ�.
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nModes, displayModeList);
		if (FAILED(hr))
		{
			return false;
		}

		// ���� ��� ���÷��� ��带 ���캸�� ȭ���� �ػ󵵿� ��ġ�ϴ� ���� ã���ϴ�.
		// ��ġ�ϴ� ���� ã���� ������� ���ΰ�ħ ���� numerator, denominator�� �����մϴ�.
		for (size_t i = 0; i < nModes; ++i)
		{
			if (displayModeList[i].Width == clientWidth && displayModeList[i].Height == clientHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}

		// �׸��� ���������� ����͸� ����Ͽ� �׷���ī���� �̸��� �޸𸮸� �����մϴ�.
		hr = adapter->GetDesc(&adapterDesc);
		if (FAILED(hr))
		{
			return false;
		}

		// �׷���ī���� �޸𸮸� mb�� �����մϴ�
		mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// �׷���ī���� �̸��� �����մϴ�.
		size_t i;
		if (0 != wcstombs_s(&i, mVideoCardDescription, 128, adapterDesc.Description, 128))
		{
			return false;
		}

		// ������� ���丮�� ����͵��� ������ �ݴϴ�.
		delete[] displayModeList;
		displayModeList = 0;

		adapterOutput->Release();
		adapterOutput = nullptr;

		adapter->Release();
		adapter = nullptr;

		factory->Release();
		factory = nullptr;



		UINT createDeviceFlag = NULL;

#ifdef _DEGUB
		createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[]
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};

		UINT driverTypeCount = sizeof(driverTypes) / sizeof(D3D_DRIVER_TYPE);

		D3D_FEATURE_LEVEL featureLevels[]
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		UINT featureLevelCount = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));

		if (mbVsync)
		{
			sd.BufferDesc.RefreshRate.Numerator = numerator;
			sd.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			sd.BufferDesc.RefreshRate.Numerator = 0;
			sd.BufferDesc.RefreshRate.Denominator = 1;
		}


		sd.BufferCount = 1; // ���� �� ����
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ���� ���� RGBA 8��Ʈ�� 32��Ʈ
		sd.BufferDesc.Width = clientWidth;
		sd.BufferDesc.Height = clientHeight;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ������� ���ó�Դϴ�.
		sd.OutputWindow = hWnd; // 
		sd.SampleDesc.Count = 16; // ��Ƽ���ø� �Դϴ� (�ȼ��� ��Ƽ���� ��)
		sd.SampleDesc.Quality = -1; // D3D11_STANDARD_MULTISAMPLE_PATTERN, D3D11_CENTER_MULTISAMPLE_PATTERN �ΰ��� �ֽ��ϴ�.
		sd.Windowed = true; // â���

		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

		for (i = 0; i < driverTypeCount; i++)
		{
			driverType = driverTypes[i];

			hr = D3D11CreateDeviceAndSwapChain(
				nullptr, driverType, 0, createDeviceFlag, featureLevels, featureLevelCount
				, D3D11_SDK_VERSION, &sd, &mSwapChain, &mDevice, &featureLevel, &mDeviceContext);

			if (SUCCEEDED(hr))
			{
				break;
			}
		}

		if (FAILED(hr))
		{
			return false;
		}


		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		hr = mDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
		{
			return false;
		}

		mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);

		D3D11_VIEWPORT view;
		view.Height = (float)clientHeight;
		view.Width = (float)clientWidth;
		view.MaxDepth = 1.f;
		view.MinDepth = 0.f;
		view.TopLeftX = 0.f;
		view.TopLeftY = 0.f;

		mDeviceContext->RSSetViewports(1, &view);

		ID3DBlob* pVSBlob = nullptr;
		hr = D3DX11CompileFromFile(L"HLSL.fx", nullptr, nullptr, "VS", "vs_4_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pVSBlob, nullptr, NULL);
		if (FAILED(hr))
		{
			return false;
		}
		hr = mDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &mVertexShader);
		if (FAILED(hr))
		{
			pVSBlob->Release();
			return false;
		}

		ID3DBlob* pPSBlob = nullptr;
		hr = D3DX11CompileFromFile(L"HLSL.fx", nullptr, nullptr, "PS", "ps_4_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0, nullptr, &pPSBlob, nullptr, NULL);
		if (FAILED(hr))
		{
			return false;
		}
		hr = mDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mPixelShader);
		pPSBlob->Release();
		if (FAILED(hr))
		{
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		UINT elementsCount = 2;
		hr = mDevice->CreateInputLayout(layout, elementsCount, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mVertexLayout);
		pVSBlob->Release();
		if (FAILED(hr))
		{
			return false;
		}

		mDeviceContext->IASetInputLayout(mVertexLayout);


		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = mDevice->CreateBuffer(&bd, nullptr, &mConstantBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		mCamera.SetCamera(XMVectorSet(0.0f, 2.0f, -10.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		mView = mCamera.GetViewMatrix();
		mProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, clientWidth / (float)clientHeight, 0.01f, 100.0f);
		return LoadAsset();
	}

	bool D3D::LoadAsset()
	{
		HRESULT hr;
		hr = mMesh.Initialize(mDevice);
		mMesh.Load(mDeviceContext);
		mMesh.SetPosition(XMVectorSet(3.0f, 0.0f, 1.0f, 0.0f));

		TimerManager::GetInstance().DeltaTimeTimer.InitTime();
		return true;
	}

	void D3D::ReleaseAsset()
	{
		mMesh.Release();
	}

	void D3D::Release()
	{
		if (mConstantBuffer != nullptr)
		{
			mConstantBuffer->Release();
			mConstantBuffer = nullptr;
		}
		if (mVertexLayout != nullptr)
		{
			mVertexLayout->Release();
			mVertexLayout = nullptr;
		}
		if (mVertexShader != nullptr)
		{
			mVertexShader->Release();
			mVertexShader = nullptr;
		}
		if (mPixelShader != nullptr)
		{
			mPixelShader->Release();
			mPixelShader = nullptr;
		}
		if (mSwapChain != nullptr)
		{
			mSwapChain->SetFullscreenState(false, nullptr);
			mSwapChain = nullptr;
		}
		if (mRenderTargetView != nullptr)
		{
			mRenderTargetView->Release();
			mRenderTargetView = nullptr;
		}
		if (mSwapChain != nullptr)
		{
			mSwapChain->Release();
			mSwapChain = nullptr;
		}
		if (mDeviceContext != nullptr)
		{
			mDeviceContext->Release();
			mDeviceContext = nullptr;
		}
		if (mDevice != nullptr)
		{
			mDevice->Release();
			mDevice = nullptr;
		}
	}

	void D3D::BeginScene(float red, float green, float blue, float alpha)
	{
		TimerManager::GetInstance().DeltaTimeTimer.SetTime();
		float deltaTime = TimerManager::GetInstance().DeltaTimeTimer.GetTime();
		const float DGREE = 100.0f;

		float color[] = { red, green, blue, alpha };
		mDeviceContext->ClearRenderTargetView(mRenderTargetView, color);

		ConstantBuffer cb;

		if (GetAsyncKeyState('A') & 0x8000)
		{
			mCamera.RotateCamera(0.0f, -DGREE * deltaTime, 0.0f);
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			mCamera.RotateCamera(0.0f, DGREE * deltaTime, 0.0f);
		}
		if (GetAsyncKeyState('W') & 0x8000)
		{
			mCamera.RotateCamera(-DGREE * deltaTime, 0.0f, 0.0f);
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			mCamera.RotateCamera(DGREE * deltaTime, 0.0f, 0.0f);
		}

		if (GetAsyncKeyState('J') & 0x8000)
		{
			mMesh.Move(-5.0f * deltaTime, 0.0f, 0.0f);
		}
		else if (GetAsyncKeyState('L') & 0x8000)
		{
			mMesh.Move(5.0f * deltaTime, 0.0f, 0.0f);
		}
		if (GetAsyncKeyState('I') & 0x8000)
		{
			mMesh.Move(0.0f, 0.0f, 5.0f * deltaTime);
		}
		else if (GetAsyncKeyState('K') & 0x8000)
		{
			mMesh.Move(0.0f, 0.0f, -5.0f * deltaTime);
		}

		mMesh.Rotate(180 * deltaTime * RADIAN, 360 * deltaTime * RADIAN, 0.0f);

		mWorld = mMesh.GetWorldMatrix();
		mView = mCamera.GetViewMatrix();
		cb.World = XMMatrixTranspose(mWorld);
		cb.View = XMMatrixTranspose(mView);
		cb.Projection = XMMatrixTranspose(mProjection);
		mDeviceContext->UpdateSubresource(mConstantBuffer, 0, nullptr, &cb, 0, 0);

		mDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
		mDeviceContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);
		mDeviceContext->PSSetShader(mPixelShader, nullptr, 0);
		mDeviceContext->DrawIndexed(mMesh.GetIndexCount(), 0, 0);
	}

	void D3D::EndScene()
	{
		if (mbVsync)
		{
			mSwapChain->Present(1, 0);
		}
		else
		{
			mSwapChain->Present(0, 0);
		}
	}
}

