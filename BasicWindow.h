#ifndef _BASIC_WINDOW_H_
#define _BASIC_WINDOW_H_


#include <Windows.h>

namespace radx
{
	static const unsigned int CLIENT_WIDTH = 800;
	static const unsigned int CLIENT_HEIGHT = 600;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	class BasicWindow
	{

	public:
		BasicWindow() = delete;
		BasicWindow(const BasicWindow& other) = delete;
		BasicWindow operator=(const BasicWindow& rhs) = delete;

		BasicWindow(HINSTANCE hInstance, LPCTSTR szWndName);
		virtual ~BasicWindow();

		bool InitWindow();

		virtual int Run();
		virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		virtual bool RegisterWindowClass();
		virtual bool CreateWnd();

		HINSTANCE mhInst;
		HWND mhWnd;
		LPCTSTR mszWndName;
	};

	static BasicWindow *gAppHandle = NULL;

}

#endif