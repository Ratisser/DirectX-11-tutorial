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
		if (false == mD3D->Initialize(mhWnd, true, mClientWidth, mClientHeight))
		{
			mD3D->Release();
			_aligned_free(mD3D);
			mD3D = nullptr;
			return false;
		}

		return true;
	}

	//void RADXSystem::ChangeScreenMode(bool bFullScreen)
	//{
	//	DEVMODE devMode = { 0 };

	//	RECT rect;

	//	if (bFullScreen)
	//	{
	//		SetWindowLong(mhWnd, GWL_STYLE, WS_POPUP);
	//		SetWindowPos(mhWnd, 0, 0, 0, mClientWidth, mClientHeight, SWP_SHOWWINDOW);
	//		devMode.dmPelsWidth = mClientWidth;
	//		devMode.dmPelsHeight = mClientHeight;
	//		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	//		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	//	}
	//	else {
	//		ChangeDisplaySettings(NULL, 0);
	//		SetWindowLong(mhWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

	//		// 윈도우 크기 설정
	//		SetRect(&rect, 0, 0, mClientWidth, mClientHeight);
	//		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	//		SetWindowPos(mhWnd, 0, 100, 100, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
	//	}
	//}

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
		return msg.wParam;
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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}