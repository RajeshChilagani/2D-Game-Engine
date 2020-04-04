#include <assert.h>
#include <Windows.h>

#include "Console\ConsolePrint.h"

namespace GLib
{
	HINSTANCE				g_hInstance = nullptr;
	HWND					g_hWnd = nullptr;

	LRESULT CALLBACK WndProc( HWND i_hWnd, UINT i_Message, WPARAM i_wParam, LPARAM i_lParam )
	{
		switch( i_Message )
		{
		case WM_PAINT:
		{
			PAINTSTRUCT			ps;
			
			HDC hdc = BeginPaint( i_hWnd, &ps );
			EndPaint( i_hWnd, &ps );
			break;
		}
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		default:
			return DefWindowProc( i_hWnd, i_Message, i_wParam, i_lParam );
		}

		return 0;
	}

	HWND _CreateWindow( WNDPROC i_pWndProc, HINSTANCE i_hInstance, WORD i_IconID, LPCTSTR i_pWindowName, unsigned int i_Width, unsigned int i_Height )
	{
		const LPCWSTR		pWindowClass = L"GLibWindowClass";

		if( i_hInstance == NULL )
			i_hInstance = reinterpret_cast<HINSTANCE>( GetModuleHandle( NULL ) );
		assert( i_hInstance != NULL );

		WNDCLASSEX			wcex;
		wcex.cbSize = sizeof( WNDCLASSEX );
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = i_pWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = i_hInstance;
		wcex.hIcon = i_IconID >= 0 ? LoadIcon( i_hInstance, MAKEINTRESOURCE( i_IconID ) ) : nullptr;
		wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
		wcex.hbrBackground = (HBRUSH) ( COLOR_WINDOW + 1 );
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = pWindowClass;
		wcex.hIconSm = i_IconID >= 0 ? LoadIcon( i_hInstance, MAKEINTRESOURCE( i_IconID ) ) : nullptr;
		if( !RegisterClassEx( &wcex ) )
		{
			DEBUG_PRINT( "Failure calling RegisterClassEx.\n" );
			return NULL;
		}

		// Create window
		RECT			rc = { 0, 0, LONG( i_Width ), LONG( i_Height ) };

		BOOL bResult = AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
		assert( bResult );

		HWND hWindow = CreateWindow( pWindowClass, i_pWindowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
						rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, i_hInstance, nullptr );

		return hWindow;
	}
} // namespace GLib