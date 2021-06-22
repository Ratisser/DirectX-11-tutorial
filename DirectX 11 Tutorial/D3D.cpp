#include "pch.h"
#include "D3D.h"

namespace radx
{
	HRESULT	CompileShaderFromFile(LPCTSTR pFileName, const D3D_SHADER_MACRO* pDefines,
		LPCSTR pEntryPoint, LPCSTR pShaderModel,
		ID3DBlob** ppBytecodeBlob)
	{
		UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob = NULL;

		HRESULT result = D3DCompileFromFile(pFileName, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			pEntryPoint, pShaderModel, compileFlags, 0,
			ppBytecodeBlob, &pErrorBlob);
		if (FAILED(result))
		{
			if (pErrorBlob != NULL)
				OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		}

		if (pErrorBlob != NULL)
			pErrorBlob->Release();

		return result;
	}

	D3D::D3D()
		: mClientWidth(800U)
		, mClientHeight(600U)
		, mFrameRate(0)
		, mbVsync(NULL)
		, mVideoCardMemory(NULL)
		, mhWnd(NULL)
		, mpDevice(nullptr)
		, mpDeviceContext(nullptr)
		, mpSwapChain(nullptr)
		, mpRenderTargetView(nullptr)
		, mpDepthStencil(nullptr)
		, mpDepthStencilView(nullptr)
		, mpVertexShader(nullptr)
		, mpConstantBuffer(nullptr)
		, mpLightBuffer(nullptr)
		, mpCameraBuffer(nullptr)
		, mpPixelShader(nullptr)
		, mpSamplerState(nullptr)
		, mProjection(XMMatrixIdentity())
		, mView(XMMatrixIdentity())
		, mWorld(XMMatrixIdentity())
		, mpVertexLayout(nullptr)
	{
		ZeroMemory(mVideoCardDescription, VIDEO_CARD_DESC_SIZE);
	}

	D3D::~D3D()
	{
	}

	bool D3D::Initialize(HWND hWnd, bool bVsync, UINT clientWidth, UINT clientHeight)
	{
		HRESULT hr;
		mhWnd = hWnd;
		mClientWidth = clientWidth;
		mClientHeight = clientHeight;
		mbVsync = bVsync;

		SetFrameRate();

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
			sd.BufferDesc.RefreshRate.Numerator = mFrameRate;
			sd.BufferDesc.RefreshRate.Denominator = 1;
		}
		else
		{
			sd.BufferDesc.RefreshRate.Numerator = 0;

		}

		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferCount = 1; // 단일 백 버퍼
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼 포맷 RGBA 8비트씩 32비트
		sd.BufferDesc.Width = mClientWidth;
		sd.BufferDesc.Height = mClientHeight;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 백버퍼의 사용처입니다.
		sd.OutputWindow = mhWnd; // 
		sd.SampleDesc.Count = 1; // 멀티샘플링 입니다 (픽셀당 멀티샘플 수)
		sd.SampleDesc.Quality = 0; // D3D11_STANDARD_MULTISAMPLE_PATTERN, D3D11_CENTER_MULTISAMPLE_PATTERN 두개가 있습니다.
		sd.Windowed = true; // 창모드

		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

		for (UINT i = 0; i < driverTypeCount; i++)
		{
			driverType = driverTypes[i];

			hr = D3D11CreateDeviceAndSwapChain(
				nullptr, driverType, 0, createDeviceFlag, featureLevels, featureLevelCount
				, D3D11_SDK_VERSION, &sd, &mpSwapChain, &mpDevice, &featureLevel, &mpDeviceContext);

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
		hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)& pBackBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		hr = mpDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
		{
			return false;
		}

		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = mClientWidth;
		descDepth.Height = mClientHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = mpDevice->CreateTexture2D(&descDepth, NULL, &mpDepthStencil);
		if (FAILED(hr))
		{
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = mpDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);
		if (FAILED(hr))
		{
			return false;
		}

		mpDeviceContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);

		D3D11_VIEWPORT view;
		view.Height = (float)clientHeight;
		view.Width = (float)clientWidth;
		view.MaxDepth = 1.0f;
		view.MinDepth = 0.0f;
		view.TopLeftX = 0.0f;
		view.TopLeftY = 0.0f;

		mpDeviceContext->RSSetViewports(1, &view);

		ID3DBlob* pVSBlob = nullptr;
    	hr = radx::CompileShaderFromFile(L"Texture.fx", nullptr, "VS", "vs_4_0", &pVSBlob);
		if (FAILED(hr))
		{
			return false;
		}

		hr = mpDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &mpVertexShader);
		if (FAILED(hr))
		{
			pVSBlob->Release();
			return false;
		}

		ID3DBlob* pPSBlob = nullptr;
		hr = radx::CompileShaderFromFile(L"Texture.fx", nullptr, "PS", "ps_4_0", &pPSBlob);
		if (FAILED(hr))
		{
			return false;
		}
		hr = mpDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);
		pPSBlob->Release();
		pPSBlob = nullptr;
		if (FAILED(hr))
		{
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		UINT elementsCount = ARRAYSIZE(layout);
		hr = mpDevice->CreateInputLayout(layout, elementsCount, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mpVertexLayout);
		pVSBlob->Release();
		pVSBlob = nullptr;
		if (FAILED(hr))
		{
			return false;
		}

		mpDeviceContext->IASetInputLayout(mpVertexLayout);

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = mpDevice->CreateBuffer(&bd, nullptr, &mpConstantBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(LightBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = mpDevice->CreateBuffer(&bd, nullptr, &mpLightBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CameraBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hr = mpDevice->CreateBuffer(&bd, nullptr, &mpCameraBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = mpDevice->CreateSamplerState(&samplerDesc, &mpSamplerState);
		if (FAILED(hr))
		{
			return false;
		}


		return LoadAsset();
	}

	bool D3D::SetFrameRate()
	{
		// Direct3D를 초기화하기 전 그래픽 카드와 모니터에서 refresh rate를 얻어야 한다고 함.
		// 그렇지 않고 refresh rate를 임의로 설정하면 DirectX가 버퍼 flip 대신 blt하여 성능이 저하되고 디버그 모드에 에러를 발생시킴

		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		IDXGIFactory* factory;

		DXGI_MODE_DESC* displayModeList;
		UINT nModes = 0;
		DXGI_ADAPTER_DESC adapterDesc;

		UINT numerator = 60;
		UINT denominator = 1;
		HRESULT hr = S_OK;

		// DirextX Graphics Interface Factory 를 생성합니다.
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)& factory);
		if (FAILED(hr))
		{
			return false;
		}

		// 팩토리를 이용해서 주 그래픽카드의 어댑터를 생성합니다.
		hr = factory->EnumAdapters(0, &adapter);
		if (FAILED(hr))
		{
			return false;
		}

		// 기본 어댑터의 출력을 나열합니다.
		hr = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(hr))
		{
			return false;
		}

		// 어댑터의 출력에 대한 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수를 가져옵니다.
		// (UTF8문자열을 UTF16문자열로 바꿀때 처럼 일단 사이즈를 먼저 구하므로
		// 마지막 인자에 NULL을 삽입)
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nModes, nullptr);
		if (FAILED(hr))
		{
			return false;
		}

		// 모니터와 그래픽 카드 조합에 가능한 모든 표시 모드를 가진 목록을 만듭니다.
		displayModeList = new DXGI_MODE_DESC[nModes];
		if (!displayModeList)
		{
			return false;
		}

		// 디스플레이 모드 목록 구조를 채웁니다.
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nModes, displayModeList);
		if (FAILED(hr))
		{
			return false;
		}

		// 이제 모든 디스플레이 모드를 살펴보고 화면의 해상도와 일치하는 것을 찾습니다.
		// 일치하는 것을 찾으면 모니터의 새로고침 빈도의 numerator, denominator를 저장합니다.
		for (size_t i = 0; i < nModes; ++i)
		{
			if (displayModeList[i].Width == mClientWidth && displayModeList[i].Height == mClientHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}

		// 그리고 마지막으로 어댑터를 사용하여 그래픽카드의 이름과 메모리를 저장합니다.
		hr = adapter->GetDesc(&adapterDesc);
		if (FAILED(hr))
		{
			return false;
		}

		// 그래픽카드의 메모리를 mb로 저장합니다
		mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// 그래픽카드의 이름을 저장합니다.
		size_t i;
		if (0 != wcstombs_s(&i, mVideoCardDescription, 128, adapterDesc.Description, 128))
		{
			return false;
		}

		// 사용헀던 팩토리와 어댑터들을 해제해 줍니다.
		delete[] displayModeList;
		displayModeList = 0;

		adapterOutput->Release();
		adapterOutput = nullptr;

		adapter->Release();
		adapter = nullptr;

		factory->Release();
		factory = nullptr;
		return false;
	}

	bool D3D::LoadAsset()
	{
		mCamera.SetCamera(XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		mView = mCamera.GetViewMatrix();
		mProjection = XMMatrixPerspectiveFovLH(/*XM_PIDIV2*/1.221730476f, mClientWidth / (float)mClientHeight, 0.01f, 100.0f);


		mMesh.Initialize(mpDevice);
		mMesh.Load(mpDeviceContext);
		mMesh.SetPosition(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		mMesh.SetScale(0.1f, 0.1f, 0.1f);

		mMesh2.Initialize(mpDevice);
		mMesh2.Load(mpDeviceContext);
		mMesh2.SetPosition(XMVectorSet(5.0f, 0.0f, 0.0f, 0.0f));
		mMesh2.SetScale(0.1f, 0.1f, 0.1f);

		mMesh3.Initialize(mpDevice);
		mMesh3.Load(mpDeviceContext);
		mMesh3.SetPosition(XMVectorSet(0.0f, -5.0f, 2.0f, 0.0f));
		mMesh3.SetScale(0.1f, 0.1f, 0.1f);
		mMesh3.Rotate(90.0f * RADIAN, 0.0f, 0.0f);

		TimerManager::GetInstance().DeltaTimeTimer.InitTime();
		TimerManager::GetInstance().DeltaTimeTimer.StartTimer();

		return true;
	}



	void D3D::updateScene()
	{
		TimerManager::GetInstance().DeltaTimeTimer.ResetTime();
		float deltaTime = TimerManager::GetInstance().DeltaTimeTimer.GetTime();
		const float DGREE = 100.0f * RADIAN;

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			mCamera.RotateCamera(0.0f, -DGREE * deltaTime, 0.0f);
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			mCamera.RotateCamera(0.0f, DGREE * deltaTime, 0.0f);
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			mCamera.RotateCamera(-DGREE * deltaTime, 0.0f, 0.0f);
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			mCamera.RotateCamera(DGREE * deltaTime, 0.0f, 0.0f);
		}
		if (GetAsyncKeyState('W') & 0x8000)
		{
			mCamera.MoveForward(50.0f * deltaTime);
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			mCamera.MoveBackward(50.0f * deltaTime);
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			mCamera.StrafeLeft(50.0f * deltaTime);
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			mCamera.StrafeRight(50.0f * deltaTime);
		}

		mMesh.Rotate(0.0f, 0.0f, 360.0f * deltaTime * RADIAN);
		//mMesh2.Rotate(0.0f, -180 * deltaTime * RADIAN, 0.0f);

		if (GetAsyncKeyState('Z') & 0x8000)
		{
			TimerManager::GetInstance().DeltaTimeTimer.StopTimer();
		}
		else if (GetAsyncKeyState('X') & 0x8000)
		{
			TimerManager::GetInstance().DeltaTimeTimer.StartTimer();
			TimerManager::GetInstance().DeltaTimeTimer.GetTime();
			TimerManager::GetInstance().DeltaTimeTimer.ResetTime();
		}

		if (GetAsyncKeyState('C') & 0x8000)
		{
			
		}
	}

	void D3D::Frame()
	{
		updateScene();

		static UINT frameCount = 0;
		static float timeElapsed = TimerManager::GetInstance().SystemTimer.GetTime();

		beginScene(0.2f, 0.2f, 0.5f, 1.0f);

		EndScene();

		if (TimerManager::GetInstance().SystemTimer.GetTime() - timeElapsed >= 1.0f)
		{
			float fps = (float)frameCount;
			float msPerFrame = 1000.0f / fps;

			std::wostringstream out;
			out.precision(6);
			out << L"FPS: " << fps << L" Frame Time: " << msPerFrame << L"ms" << L" VideoCard: " << GetVideoCardDescription();
			SetWindowText(mhWnd, out.str().c_str());

			frameCount = 0;
			timeElapsed = TimerManager::GetInstance().SystemTimer.GetTime();
		}

		frameCount++;
	}

	void D3D::beginScene(float red, float green, float blue, float alpha)
	{
		float color[] = { red, green, blue, alpha };
		mpDeviceContext->ClearRenderTargetView(mpRenderTargetView, color);
		mpDeviceContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		ConstantBuffer cb;
		mWorld = mMesh.GetWorldMatrix();
		mView = mCamera.GetViewMatrix();
		cb.World = XMMatrixTranspose(mWorld);
		cb.View = XMMatrixTranspose(mView);
		cb.Projection = XMMatrixTranspose(mProjection);
		mpDeviceContext->UpdateSubresource(mpConstantBuffer, 0, nullptr, &cb, 0, 0);

		LightBuffer lb;
		lb.LightPos.m128_f32[0] = 500.0f;
		lb.LightPos.m128_f32[1] = 500.0f;
		lb.LightPos.m128_f32[2] = 500.0f;
		lb.LightPos.m128_f32[3] = 0.0f;
		mpDeviceContext->UpdateSubresource(mpLightBuffer, 0, nullptr, &lb, 0, 0);

		CameraBuffer cam;
		cam.CamPos = mCamera.GetPosition();
		mpDeviceContext->UpdateSubresource(mpCameraBuffer, 0, nullptr, &cam, 0, 0);

		mpDeviceContext->VSSetShader(mpVertexShader, nullptr, 0);
		mpDeviceContext->VSSetConstantBuffers(0, 1, &mpConstantBuffer);
		mpDeviceContext->VSSetConstantBuffers(1, 1, &mpLightBuffer);
		mpDeviceContext->VSSetConstantBuffers(2, 1, &mpCameraBuffer);

		// 텍스쳐 설정
		ID3D11ShaderResourceView* tex = mMesh.GetTexture();
		mpDeviceContext->PSSetShaderResources(0, 1, &tex);
		mpDeviceContext->PSSetSamplers(1, 1, &mpSamplerState);
		// 여기까지

		mpDeviceContext->PSSetShader(mpPixelShader, nullptr, 0);
		mpDeviceContext->Draw(mMesh.GetVertexCount(), 0);
		//mpDeviceContext->DrawIndexed(mMesh.GetIndexCount(), 0, 0);

		static float angle = 0.0f;
		angle += 180.0f * RADIAN * TimerManager::GetInstance().DeltaTimeTimer.GetTime();
		mWorld = mMesh2.GetWorldMatrix() * XMMatrixRotationY(angle);

		cb.World = XMMatrixTranspose(mWorld);
		mpDeviceContext->UpdateSubresource(mpConstantBuffer, 0, nullptr, &cb, 0, 0);
		mpDeviceContext->Draw(mMesh2.GetVertexCount(), 0);
		//mpDeviceContext->DrawIndexed(mMesh2.GetIndexCount(), 0, 0);

		cb.World = XMMatrixTranspose(mMesh3.GetWorldMatrix());
		mpDeviceContext->UpdateSubresource(mpConstantBuffer, 0, nullptr, &cb, 0, 0);
		mpDeviceContext->Draw(mMesh3.GetVertexCount(), 0);
		//mpDeviceContext->DrawIndexed(mMesh3.GetIndexCount(), 0, 0);

	}

	void D3D::EndScene()
	{
		if (mbVsync)
		{
			mpSwapChain->Present(1, 0);
		}
		else
		{
			mpSwapChain->Present(0, 0);
		}
	}

	const char* D3D::GetVideoCardDescription()
	{
		return mVideoCardDescription;
	}

	void D3D::ReleaseAsset()
	{
		mMesh.Release();
		mMesh2.Release();
		mMesh3.Release();
	}

	void D3D::Release()
	{
		ReleaseAsset();

		if (mpSamplerState != nullptr)
		{
			mpSamplerState->Release();
			mpSamplerState = nullptr;
		}
		if (mpConstantBuffer != nullptr)
		{
			mpConstantBuffer->Release();
			mpConstantBuffer = nullptr;
		}
		if (mpLightBuffer)
		{
			mpLightBuffer->Release();
			mpLightBuffer = nullptr;
		}
		if (mpCameraBuffer)
		{
			mpCameraBuffer->Release();
			mpCameraBuffer = nullptr;
		}
		if (mpVertexLayout != nullptr)
		{
			mpVertexLayout->Release();
			mpVertexLayout = nullptr;
		}
		if (mpVertexShader != nullptr)
		{
			mpVertexShader->Release();
			mpVertexShader = nullptr;
		}
		if (mpPixelShader != nullptr)
		{
			mpPixelShader->Release();
			mpPixelShader = nullptr;
		}
		if (mpSwapChain != nullptr)
		{
			mpSwapChain->SetFullscreenState(false, nullptr);
			mpSwapChain = nullptr;
		}
		if (mpDepthStencil != nullptr)
		{
			mpDepthStencil->Release();
			mpDepthStencil = nullptr;
		}
		if (mpDepthStencilView != nullptr)
		{
			mpDepthStencilView->Release();
			mpDepthStencilView = nullptr;
		}
		if (mpRenderTargetView != nullptr)
		{
			mpRenderTargetView->Release();
			mpRenderTargetView = nullptr;
		}
		if (mpSwapChain != nullptr)
		{
			mpSwapChain->Release();
			mpSwapChain = nullptr;
		}
		if (mpDeviceContext != nullptr)
		{
			mpDeviceContext->Release();
			mpDeviceContext = nullptr;
		}
		if (mpDevice != nullptr)
		{
			mpDevice->Release();
			mpDevice = nullptr;
		}

		_aligned_free(this);
	}
}

