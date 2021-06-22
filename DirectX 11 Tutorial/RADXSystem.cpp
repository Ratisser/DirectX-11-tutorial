#include "pch.h"
#include "RADXSystem.h"

namespace radx
{
	RADXSystem::RADXSystem(HINSTANCE hInstance, LPCTSTR szWndName, UINT clientWidth, UINT clientHeight)
		: BasicWindow(hInstance, szWndName)
		, mClientWidth(clientWidth)
		, mClientHeight(clientHeight)
		, mD3D(nullptr)
	{
	}

	RADXSystem::~RADXSystem()
	{
	}

	bool RADXSystem::Initialize()
	{
		TimerManager::GetInstance().SystemTimer.InitTime();
		TimerManager::GetInstance().SystemTimer.ResetTime();
		TimerManager::GetInstance().SystemTimer.StartTimer();

		if (false == InitWindow())
		{
			return false;
		}

		mD3D = (D3D*)_aligned_malloc(sizeof(D3D), 16);
		if (false == mD3D->Initialize(mhWnd, false, mClientWidth, mClientHeight))
		{
			mD3D->Release();
			_aligned_free(mD3D);
			mD3D = nullptr;
			return false;
		}

		return true;
	}



	int RADXSystem::Run()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if (mD3D)
				{
					mD3D->Frame();
				}
			}
		}

		mD3D->Release();
		mD3D = nullptr;
		return (int)msg.wParam;
	}

	LRESULT RADXSystem::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_SIZE:
			
			break;
		//case WM_MOVE:
		//	break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_KEYDOWN:

			break;
		case WM_KEYUP:
			break;
		case WM_QUIT:

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}