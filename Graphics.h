#pragma once

#include "D3D.h"

namespace radx
{
	class Graphics
	{
	public:
		Graphics();
		~Graphics();

		bool				Initialize(HWND hWnd, UINT clientWidth, UINT clientHeight);
		void				Release();
		
		void				Frame();
		
	private:
		D3D*				mD3D;
		HWND				mhWnd;
	};
}
