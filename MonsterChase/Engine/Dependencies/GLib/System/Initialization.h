#pragma once
#include <Windows.h>

namespace GLib
{
	bool Initialize( HINSTANCE i_hInstance, int i_CmdShow, const char * i_pWindowName, WORD i_IconID, unsigned int i_WindowWidth, unsigned int i_WindowHeight, bool i_bDebugDevice );
	void Shutdown();
} // namespace GLib