#include "pch.h"
#include "RADXSystem.h"

namespace radx
{
	RADXSystem::RADXSystem(HINSTANCE hInstance, LPCTSTR szWndName, UINT clientWidth, UINT clientHeight)
		: BasicWindow(hInstance, szWndName)
		, mClientWidth(clientWidth)
		, mClientHeight(clientHeight)
	{
	}

	RADXSystem::~RADXSystem()
	{
	}

	bool RADXSystem::Initialize()
	{
		mInput = std::make_unique<Input>();
		mGraphics = std::make_unique<Graphics>();

		if (false == InitWindow())
		{
			return false;
		}

		if (false == mGraphics->Initialize(mhWnd, mClientWidth, mClientHeight))
		{
			return false;
		}

		return true;
	}

	void RADXSystem::ChangeScreenMode(bool bFullScreen)
	{
		DEVMODE devMode = { 0 };

		RECT rect;

		if (bFullScreen)
		{
			SetWindowLong(mhWnd, GWL_STYLE, WS_POPUP);
			SetWindowPos(mhWnd, 0, 0, 0, mClientWidth, mClientHeight, SWP_SHOWWINDOW);
			devMode.dmPelsWidth = mClientWidth;
			devMode.dmPelsHeight = mClientHeight;
			devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
			ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		}
		else {
			ChangeDisplaySettings(NULL, 0);
			SetWindowLong(mhWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

			// 윈도우 크기 설정
			SetRect(&rect, 0, 0, mClientWidth, mClientHeight);
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			SetWindowPos(mhWnd, 0, 100, 100, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		}
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
				mGraphics->Frame();
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
		case WM_MOVE:
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_KEYDOWN:
			break;
		case WM_KEYUP:
			break;
		default:
			break;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}