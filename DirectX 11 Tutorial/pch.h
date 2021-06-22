#pragma once

#include <Windows.h>
#include <tchar.h>
#include <cassert>
#include <memory>
#include <D3D11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>
#include <sstream>
#include <d3dcompiler.h>


#include "D3DX11.h"
#include "TimerManager.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler")

#define WIN32_LEAN_AND_MEAN
#define RADIAN 0.017453293F


using namespace DirectX;