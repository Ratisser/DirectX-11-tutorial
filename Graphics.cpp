#include "pch.h"
#include "Graphics.h"

namespace radx
{

	Graphics::Graphics()
		: mD3D(nullptr)
		, mhWnd(0)
	{
	}


	Graphics::~Graphics()
	{
	}

	bool Graphics::Initialize(HWND hWnd, UINT clientWidth, UINT clientHeight)
	{
		bool bResult;
		mhWnd = hWnd;
		TimerManager::GetInstance().SystemTimer.InitTime();
		TimerManager::GetInstance().SystemTimer.SetTime();

		mD3D = (D3D*)_aligned_malloc(sizeof(D3D), 16);

		bResult = mD3D->Initialize(hWnd, false, clientWidth, clientHeight);
		//assert(bResult == true && "could not initialize Direct3D.");
		if (!bResult)
		{
			MessageBox(hWnd, L"Direct3D�� �ʱ�ȭ �� �� �������ϴ�.", L"����", MB_OK);
			mD3D->Release();
			return false;
		}

		return true;
	}

	void Graphics::Release()
	{
		if (mD3D)
		{
			mD3D->Release();
			_aligned_free(mD3D);
			mD3D = nullptr;
		}
	}

	void Graphics::Render()
	{
		static UINT frameCount = 0;
		static float timeElapsed = TimerManager::GetInstance().SystemTimer.GetTime();

		// ���� �����ϱ� ���� ���۸� ���ϴ�.
		mD3D->BeginScene(0.2f, 0.2f, 0.5f, 1.0f);

		// ȭ�鿡 �������� ���� ������Ʈ �մϴ�.
		mD3D->EndScene();

		if (TimerManager::GetInstance().SystemTimer.GetTime() - timeElapsed >= 1.0f)
		{
			float fps = (float)frameCount;
			float msPerFrame = 1000.0f / fps;

			std::wostringstream out;
			out.precision(6);
			out << L"FPS: " << fps << L" Frame Time: " << msPerFrame << L"ms";
			SetWindowText(mhWnd, out.str().c_str());

			frameCount = 0;
			timeElapsed = TimerManager::GetInstance().SystemTimer.GetTime();
		}

		frameCount++;
	}

}