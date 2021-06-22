#pragma once

#include "Input.h"
#include "BasicWindow.h"
#include "D3D.h"

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
		// d3d11 project dosen't need codes below to change full screen mode
		// void ChangeScreenMode(bool bFullScreen);

		int Run() override;

		LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:
		D3D* mD3D;

		UINT mClientWidth;
		UINT mClientHeight;
		
	};
}