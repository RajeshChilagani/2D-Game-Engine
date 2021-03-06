#include "Core/Memory/Allocatorsh.h"
#include "Engine.h"
#include "Game.h"
#include <conio.h>
#include <stdio.h>

using namespace std;
using namespace Engine;
int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	const size_t 		sizeHeap = 1024 * 1024 * 2;
	void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap); 
	Engine::EnteringMain(pHeapMemory, sizeHeap);

	if (Engine::Init())
	{
		if (Game::Init(i_hInstance,i_hPrevInstance,i_lpCmdLine,i_nCmdShow))
		{
			Game::Run();
			Game::ShutDown();
		}
		Engine::ShutDown();
	}

	Engine::LeavingMain();
_CrtDumpMemoryLeaks();
}


