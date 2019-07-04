#include "pch.h"
#include "RADXSystem.h"


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	radx::RADXSystem radx(hInstance, _T("D3D11"), 800, 600);
	if (false == radx.Initialize())
	{
		return 0;
	}
	radx.Run();

	return 0;
}