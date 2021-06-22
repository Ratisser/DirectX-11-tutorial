#include "pch.h"
#include "RADXSystem.h"

#include <crtdbg.h>

#ifndef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__,__LINE)
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	radx::RADXSystem radx(hInstance, _T("D3D11"), 800, 600);
	if (false == radx.Initialize())
	{
		return 0;
	}
	radx.Run();

	return 0;
}