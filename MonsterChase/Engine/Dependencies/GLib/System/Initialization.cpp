#include <assert.h>

#include <Windows.h>

#include "GLib.h"

#include "Device.h"
#include "DXHelpers.h"
#include "Input.h"
#include "Window.h"

#include "Console\ConsolePrint.h"
#include "Buffers\DrawResourcesBuffer.h"

namespace GLib
{
	bool Initialize(HINSTANCE i_hInstance, int i_CmdShow, const char* i_pWindowName, WORD i_IconID, unsigned int i_WindowWidth, unsigned int i_WindowHeight, bool i_bDebugDevice)
	{
		g_hInstance = i_hInstance;

#ifdef  UNICODE
		const size_t		lenWindowName = 128;
		wchar_t				WindowName[lenWindowName];

		MultiByteToWideChar(CP_ACP, 0, (i_pWindowName) ? i_pWindowName : "GLib Render", -1, WindowName, lenWindowName);

		LPCTSTR pWindowName = WindowName;
#else
		LPCTSTR pWindowName = (i_pWindowName) ? i_pWindowName : "GLib Render";
#endif // UNICODE

		g_hWnd = _CreateWindow(WndProc, g_hInstance, i_IconID, pWindowName, i_WindowWidth, i_WindowHeight);
		if (g_hWnd == nullptr)
		{
			DEBUG_PRINT("_CreateWindow() failed.\n");
			return NULL;
		}

		HRESULT hResult = CreateDevice(g_hWnd, i_bDebugDevice);
		if (!Succeeded(hResult))
		{
			DEBUG_PRINT("_CreateDevice() failed. HRESULT = %d.\n", hResult);
			return false;
		}

		ShowWindow(g_hWnd, i_CmdShow);

		Input::Initialize();
		Sprites::Initialize();
		return true;
	}

	void Shutdown()
	{
		Input::Shutdown();
		Sprites::Shutdown();
		ReleaseDevice();
	}
} // namespace GLib