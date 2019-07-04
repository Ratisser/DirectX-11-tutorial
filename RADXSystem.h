#pragma once


#include "Input.h"
#include "BasicWindow.h"
#include "Graphics.h"

namespace radx
{
	class RADXSystem : public BasicWindow
	{
	public:
		RADXSystem() = delete;
		RADXSystem(const RADXSystem& other) = delete;
		RADXSystem operator=(const RADXSystem& rhs) = delete;
		
		RADXSystem(HINSTANCE hInstance, LPCTSTR szWndName, UINT clientWidth, UINT clientHeight);
		~RADXSystem();

		bool Initialize();
		void ChangeScreenMode(bool bFullScreen);

		int Run() override;

		LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:
		std::unique_ptr<Input> mInput;
		std::unique_ptr<Graphics> mGraphics;

		UINT mClientWidth;
		UINT mClientHeight;
		
	};
}