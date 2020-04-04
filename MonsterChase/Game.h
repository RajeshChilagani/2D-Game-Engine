#pragma once
#include <stdlib.h>
#include <Windows.h>

namespace Game
{
	bool Init(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow);
	void Run();
	void ShutDown();
}