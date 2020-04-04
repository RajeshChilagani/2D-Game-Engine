#pragma once
#include <Windows.h>

namespace GLib
{
	extern HINSTANCE			g_hInstance;
	extern HWND					g_hWnd;

	LRESULT CALLBACK WndProc( HWND i_hWnd, UINT i_Message, WPARAM i_wParam, LPARAM i_lParam );
	HWND _CreateWindow( WNDPROC i_pWndProc, HINSTANCE i_hInstance, WORD i_IconID, LPCTSTR i_pWindowName, unsigned int i_Width, unsigned int i_Height );
} // namespace GLib