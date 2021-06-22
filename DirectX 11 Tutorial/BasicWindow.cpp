#include "pch.h"
#include "BasicWindow.h"

namespace radx
{

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
			break;
		case WM_COMMAND:
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:

			break;
		}

		return gAppHandle->WndProc(hWnd, message, wParam, lParam);
	}



	BasicWindow::BasicWindow(HINSTANCE hInstance, LPCTSTR szWndName)
		: mhInst(hInstance)
		, mszWndName(szWndName)
	{
	}

	BasicWindow::~BasicWindow()
	{
	}

	bool BasicWindow::InitWindow()
	{
		gAppHandle = this;

		if (!RegisterWindowClass())
		{
			return false;
		}
		if (!CreateWnd())
		{
			return false;
		}

		ShowWindow(mhWnd, SW_SHOW);
		UpdateWindow(mhWnd);


		return true;
	}

	bool BasicWindow::RegisterWindowClass()
	{
		WNDCLASSEX wc;

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_CLASSDC;
		wc.lpfnWndProc = radx::WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = mhInst;
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = mszWndName;
		wc.hIconSm = NULL;

		if (!RegisterClassEx(&wc))
		{
			return false;
		}

		return true;
	}

	bool BasicWindow::CreateWnd()
	{
		mhWnd = CreateWindow(mszWndName, mszWndName, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CLIENT_WIDTH, CLIENT_HEIGHT,
			NULL, NULL, mhInst, NULL);
		if (!mhWnd)
		{
			return false;
		}
		return true;
	}


	int BasicWindow::Run()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return (int)msg.wParam;
	}

	LRESULT BasicWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_KEYDOWN:
		case WM_KEYUP:
		default:
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}