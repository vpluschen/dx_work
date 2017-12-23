#pragma once
#include "d3d9.h"
#include "d3dx9.h"
#include <windows.h>
#include <stdint.h>

class DxUtil
{
public:
	static bool Init(HWND hWnd, bool fullScreen);
	static IDirect3DDevice9* GetDevice();
	static void Clear();
	static void Release();
};